
#include <arduino.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_ILI9341.h>
#include <EEPROM.h>
#include "i2c.h"
#include "si5351.h"
#include "measurement.h"
#include "ringmeter.h"
#include "setup.h"
#include "_EEPROMAnything.h"
#include "Cal_config.h"
#include "Fonts/FreeSans18pt7b.h"
#include "Fonts/FreeSansBold12pt7b.h"
#include "Fonts/FreeSans12pt7b.h"
#include "Fonts/FreeMono9pt7b.h"
#include "Fonts/FreeMonoBold24pt7b.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include <credentials.h>
#include "network.h"

const char* host = "esp32-vfo"; //esp32-vfo
extern  Adafruit_ILI9341 tft;

// Telnet server one for debug one for CAT interface
static WiFiServer Server(23);  

Telnet DebugServer(&Server, 1, "Connected to PA0PHH ESP-VPO Debug Console\r\n");
TelnetQueue CatServer;

QueueHandle_t     write_queue = NULL;
QueueHandle_t     read_queue = NULL;


void wifiinit(void) {
  
  char str[80], str1[80];
  int timer = 0;
  int ssid_count = 0;
  
  // Connect to WiFi network
  
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&FreeMono9pt7b);
  tft.println("");
  tft.print("Try to connect to: ");
  tft.println(ssid[ssid_count]);
  WiFi.begin(ssid[ssid_count], password[ssid_count]);
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    timer++;
    tft.print(".");
    if ((timer >= 10) && (ssid_count < NUMBR_SSID))
      {
          timer=0;
          tft.println("");
          tft.print(ssid[ssid_count]);
          tft.println(" Network not found");
          WiFi.disconnect();
          delay(500);
          ssid_count++;
          tft.print("Try to connect to: ");
          tft.println(ssid[ssid_count]);
          WiFi.begin(ssid[ssid_count], password[ssid_count]);
        }
       else if (ssid_count == NUMBR_SSID)
       {
          WiFi.disconnect();
          tft.println(" Network not found");
          delay(2000);
          return;
       }  
  }
  
  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    tft.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  
  String ip = WiFi.localIP().toString();
  tft.println("");
  ip.toCharArray(str1, 80);
  sprintf(str,"Connected to %s ", ssid[ssid_count]);
  tft.println(str);
  sprintf(str,"IP address:  %s", str1);
  tft.println(str);
  init_ota();

  DebugServer.begin();
  CatServer.begin();

  // Delay to show IP number
  delay(2000);
}


void init_ota(void)
{  
  uint16_t maxlen = strlen(host) + 7;
  char *fullhostname = new char[maxlen];
  uint8_t mac[6];
  
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3

  
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
  WiFi.macAddress(mac);
  snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", host, mac[3], mac[4], mac[5]);
  ArduinoOTA.setHostname(fullhostname);
  ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      tft.fillScreen(ILI9341_BLACK);
      tft.setFont(&FreeSans12pt7b);
      tft.setCursor(0, 0);
      tft.println(""); 
      tft.print("Start OTA: ");
      tft.println("updating " + type);
    })
    .onEnd([]() {
      tft.println("\nEnd");
      delay(1000);
    })
    .onProgress([](unsigned int progress, unsigned int total) {
        int16_t     x1, y1;
        uint16_t    w, h;

        tft.setFont(&FreeSans12pt7b);
        tft.setCursor(0, 60);
		tft.getTextBounds("Progress: 88888", 0, 50, &x1, &y1, &w, &h);
		tft.fillRect(0, 60 - h + 1, w, h, ILI9341_BLACK);
        tft.print("Progress: " + String((progress / (total / 100))));
    })
    .onError([](ota_error_t error) {
      tft.println("Error : " + String(error));
      if (error == OTA_AUTH_ERROR) tft.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) tft.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) tft.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) tft.println("Receive Failed");
      else if (error == OTA_END_ERROR) tft.println("End Failed");
    });

  ArduinoOTA.begin();
}



void network_loop() {
  ArduinoOTA.handle();
  DebugServer.handle();
}

// Constructor

Telnet::Telnet(WiFiServer *lpServer, int maxconnections, String intro) 
{
  max_connections = min(maxconnections, MAX_CLIENTS);
  pServer = lpServer;
  IntroMessage = intro;
}

// Constructor

Telnet::~Telnet() 
{
  for (int i=0 ; i < max_connections ; ++i) 
      {
        if (NULL != clients[i]) 
        {
          clients[i]->stop();
          delete clients[i];
          clients[i] = NULL;
        }
    }   
}

void Telnet::begin()
{
    if (pServer)
    {
        pServer->begin();
        pServer->setNoDelay(true);
    }
}

void Telnet::handle()
{
  CheckForConnections();
  CheckForDroppedConnections();
}

uint8_t Telnet::connected(int iclient)
{
 if ((iclient > max_connections) || (clients[iclient] == NULL))
   return 0;
 return clients[iclient]->connected();
}

uint8_t Telnet::Anyconnection()
{
 for (int i=0 ; i<max_connections ; ++i) 
    {
      if (NULL != clients[i]) 
      {
        if (clients[i]->connected())
        {
        return 1;
        }
      }
    } 
return 0;
}

int Telnet::available(int iclient)
{
 if ((iclient > max_connections) || (clients[iclient] == NULL))
    return 0;
 return clients[iclient]->available();
}

void Telnet::CheckForConnections()
{
  // CHeck if client is connecting
  WiFiClient newClient = pServer->available();
  if (newClient) 
  {
    Serial.println("new client");
    // Find the first unused space
    for (int i=0 ; i<max_connections ; ++i) 
    {
      if (NULL == clients[i]) 
      {
        clients[i] = new WiFiClient(newClient);
        clients[i]->print(IntroMessage);
        break;
      }
    }
  }
  else
  {
    newClient.stop();  
  }
}

void Telnet::CheckForDroppedConnections()
{
  for (int i=0 ; i<max_connections ; ++i) 
    {
      if (NULL != clients[i]) 
      {
        if (clients[i]->connected() == false)
        {
          Serial.println("remove client");
          clients[i]->stop();
          delete clients[i];
          clients[i] = NULL;
        }
      }
  } 
}

size_t Telnet::write(int iclient, const uint8_t *buffer, size_t size)
{
  if (iclient > max_connections)
    return 0;
  size_t ret_val = clients[iclient]->write(buffer, size);
  return ret_val;
}

void Telnet::print(int iclient , String string)
{
  if (iclient > max_connections)
    return ;
  clients[iclient]->print(string);
  return ;
}

size_t Telnet::writeAll(const uint8_t *buffer, size_t size)
{
  size_t ret_val = 0;
  for (int i=0 ; i<max_connections ; ++i) 
    {
    if (clients[i] != NULL)
      {
        if (clients[i]->connected())
        {
          ret_val =  clients[i]->write(buffer, size);
        }
      }
    }
  return ret_val;
}

size_t Telnet::read(int iclient, const uint8_t *buffer, size_t size)
{
  if ((iclient > max_connections) || (clients[iclient] == NULL))
    return 0;
  return clients[iclient]->read((uint8_t  *)buffer, size);
}

size_t Telnet::readAll(const uint8_t *buffer, size_t size)
{
  size_t ret_val = 0;
  for (int i=0 ; i<max_connections ; ++i) 
    {
    if (clients[i] == NULL)
      continue;    
    if (clients[i]->connected() && clients[i]->available())
      {
	  return clients[i]->read((uint8_t  *)buffer, size);  
      }
    }
  return ret_val;
}

void TelnetQueue::begin()
{
    write_queue = xQueueCreate(10, NET_BUFSIZE * sizeof(char));
    if (write_queue == NULL) {
        Serial.println("Error creating the queue");
    }
	read_queue = xQueueCreate(10, NET_BUFSIZE * sizeof(char));
	if (read_queue == NULL) {
		Serial.println("Error creating the queue");
	}
}

uint8_t TelnetQueue::Anyconnection()
{
    return 1;
}

size_t TelnetQueue::write(int iclient, const uint8_t* buffer, size_t size)
{

}
size_t TelnetQueue::writeAll(const uint8_t* buffer, size_t size)
{
    char str[NET_BUFSIZE+1];
    
    memset(str, 0, (NET_BUFSIZE + 1) * sizeof(char));
    strncpy(str, (char *)buffer, min((size_t)NET_BUFSIZE,size));
    str[NET_BUFSIZE - 1] = '\0';

    xQueueSend(write_queue, str, 0);
}
size_t TelnetQueue::readAll(const uint8_t* buffer, size_t size)
{
	char str[NET_BUFSIZE + 1];
	memset(str, 0, (NET_BUFSIZE + 1) * sizeof(char));
	if (xQueueReceive(read_queue, str, 0))
	{
        strcpy((char *)buffer, str);
        return strlen(str);
    }
    return 0;
}

void task1(void* arg)
{
    static WiFiServer   TRXNETServer(1003); 
    static Telnet       TelServer(&TRXNETServer, MAX_CLIENTS, "Connected to PA0PHH ESP-VPO TRXNET interface\r\n");

    TelServer.begin();
	while (1)
	{
		char str[NET_BUFSIZE+1];
		memset(str, 0, (NET_BUFSIZE + 1) * sizeof(char));
		if (xQueueReceive(write_queue, str, 0))  
		{
            TelServer.writeAll((const uint8_t*) str, strlen(str));
		}
        memset(str, 0, (NET_BUFSIZE + 1) * sizeof(char));
        if (TelServer.Anyconnection())
        {
            TelServer.readAll((const uint8_t*)str, NET_BUFSIZE);
            if (strlen(str))
            {
                xQueueSend(read_queue, &str, 0);
            }
        }
        TelServer.handle();
    	vTaskDelay(1);
	}
}
