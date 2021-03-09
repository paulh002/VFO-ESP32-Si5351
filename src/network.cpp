
/*
  network.cpp - network class for esp32 vfo and connected devices
  Copyright (c) 2021 Paul Hollander  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <arduino.h>
#include <gfxfont.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SPITFT.h>
#include <Adafruit_SPITFT_Macros.h>
#include <Adafruit_ILI9341.h>
#include "Fonts/FreeMono9pt7b.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>
#include <ArduinoOTA.h>
#include <credentials.h>
#include "network.h"

const char* host = "esp32-amp";
const char* vfo_host = "esp32-vfo-f8649";

extern  Adafruit_ILI9341 tft;

// Static  classes only used here
// Telnet server one for debug one for CAT interface
static WiFiServer           Server(23);  
static WiFiServer           TRXNETServer(1003);
static WiFiClient			TRXNETClient;
static TelnetClient			TelClient;
static QueueHandle_t		write_queue = NULL, read_queue = NULL;


// CatServer is used by VFO implementation
// CatTelCient is used by client of the VFO
// For convenient I use this implementation for both server and client both server and client are defined but only one of them used
TelnetQueue					CatServer; 
TelnetClientQueue           CatTelClient;

// DebugSever can be used to send debug messages to Telnet port 23
Telnet						DebugServer(&Server, 1, "Connected to PA0PHH ESP-VPO Debug Console\r\n");


void TrxNetwork::begin(uint8_t type_network)
{
	char str[80], str1[80];
	int timer = 0;
	int ssid_count = 0;

	// Connect to WiFi network
	tft.fillScreen(ILI9341_BLACK);
	tft.setFont(&FreeMono9pt7b);
	tft.println("");
	tft.print("Connect to: ");
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
			timer = 0;
			tft.println("");
			tft.print(ssid[ssid_count]);
			tft.println(" Network not found");
			WiFi.disconnect();
			delay(500);
			ssid_count++;
			tft.print("Connect to: ");
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

	/*use mdns for host name resolution */
	if (!MDNS.begin(host)) { //http://esp32.local
		tft.println("Error setting up MDNS responder!");
		while (1) {
			delay(1000);
		}
	}
	String ip = WiFi.localIP().toString();
	tft.println("");
	ip.toCharArray(str1, 80);
	sprintf(str, "Connected to %s ", ssid[ssid_count]);
	tft.println(str);
	sprintf(str, "IP address:  %s", str1);
	tft.println(str);
	init_ota();

	DebugServer.begin();
	if (type_network == TRXNET_CLIENT)
	{
		IPAddress vfo_serverIp;
		int timer = 0;
		
		tft.println();
		vfo_serverIp = MDNS.queryHost(vfo_host);
		tft.println("Query " + String(vfo_host));
		timer = 0;
		while (vfo_serverIp.toString() == String("0.0.0.0") && timer < 10)
		{
			delay(500);
			timer++;
			tft.print(".");
			vfo_serverIp = MDNS.queryHost(vfo_host);
		}
		timer = 0;
		if (vfo_serverIp.toString() != String("0.0.0.0"))
		{
			tft.print("Connect to : ");
			tft.println(vfo_serverIp.toString());
			timer = 0;
			while (TRXNETClient.connect(vfo_serverIp, 1003) == 0 && timer < 10)
			{
				timer++;
				tft.print(".");
				delay(500);
				TRXNETClient.connect(vfo_serverIp, 1003);
			}
		}
		if (TRXNETClient.connected())
		{
			tft.println("Host: " + String(vfo_host));
		}
		else
		{
			tft.setTextColor(ILI9341_RED);
			tft.println("Failed to host: " + String(vfo_host));
			delay(2000);
			return;
		}		
		CatTelClient.begin();
		xTaskCreatePinnedToCore(this->task1_client, "Task1", 4096, NULL, 1, &Task1, 0);
	}
	if (type_network == TRXNET_SERVER)
	{
		CatServer.begin();
		xTaskCreatePinnedToCore(this->task1_server, "Task1", 4096, NULL, 1, &Task1, 0);
	}
	// Delay to show IP number
	delay(2000);
}


void TrxNetwork::init_ota(void)
{
	uint16_t maxlen = strlen(host) + 7;
	char* fullhostname = new char[maxlen];
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
	tft.setFont(&FreeMono9pt7b);
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

			tft.setFont(&FreeMono9pt7b);
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

void TrxNetwork::network_loop() {
	ArduinoOTA.handle();
	DebugServer.handle();

// Read the debugServer buffer even if we don't do anything with it
// Otherwise disconnects are not detected
	char str[NET_BUFSIZE+1];
	memset(str, 0, sizeof(str));
	size_t retval = DebugServer.readAll((const uint8_t*)str, NET_BUFSIZE);
}

// Client thread is started when network is in client mode
// Thread to handle incoming tcp data and put them in the read queue
// The write queue is filled by CAT interface and red by this thread and send to tcp server
//
void TrxNetwork::task1_client(void* arg)
{

	TelClient.begin(&TRXNETClient);
	while (1)
	{
		char str[NET_BUFSIZE + 1];
		memset(str, 0, sizeof(str));
		if (xQueueReceive(write_queue, str, 0))
		{
			if (TelClient.connected())
			{
				TelClient.write((const uint8_t*)str, (size_t)strlen(str));
			}
		}
		memset(str, 0, sizeof(str));
		if (TelClient.connected())
		{
			TelClient.read((const uint8_t*)str, NET_BUFSIZE);
			if (strlen(str))
			{
				xQueueSend(read_queue, str, 0);
			}
		}
		TelClient.handle();
		vTaskDelay(1);
	}
}

// Server thread is started when network is in Server mode
// Thread to handle incoming tcp data from clients and put them in the read queue
// The write queue is filled by CAT interface requests and red by this thread and send to tcp client
//
void TrxNetwork::task1_server(void* arg)
{
	static Telnet       TelServer(&TRXNETServer, MAX_CLIENTS, "Connected to PA0PHH ESP-VPO TRXNET interface\r\n");

	TelServer.begin();
	while (1)
	{
		char str[NET_BUFSIZE + 1];
		memset(str, 0,  sizeof(str));
		if (xQueueReceive(write_queue, str, 0))
		{
			TelServer.writeAll((const uint8_t*)str, strlen(str));
		}
		memset(str, 0, sizeof(str));
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


// Telnet class 
// Creates a telnet server on the port configured by WifiServer class
// Multiple clients can connect defined by MAX_CLIENTS
// For each client a WifiClient class is created. 
// ReadAll reads data from first available client, it is assumed that readAll is called in a loop
// So if multiple clients are sending data all of the data is handled one by one
// WriteAll send a data message to all connected clients
// The handle function need to be called regular to check for new connections or dropped connections
//
// Constructor

Telnet::Telnet(WiFiServer *lpServer, int maxconnections, String intro) 
{
  max_connections = min(maxconnections, MAX_CLIENTS);
  pServer = lpServer;
  IntroMessage = intro;
}

// Destructor

Telnet::~Telnet() 
{
  for (int i=0 ; i < max_connections ; i++) 
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
	CheckForDroppedConnections(); 
	CheckForConnections();
}

uint8_t Telnet::connected(int iclient)
{
 if ((iclient > max_connections) || (clients[iclient] == NULL))
   return 0;
 return clients[iclient]->connected();
}

uint8_t Telnet::Anyconnection()
{
 for (int i=0 ; i<max_connections ; i++) 
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
    for (int i=0 ; i<max_connections ; i++) 
    {
      if (NULL == clients[i]) 
      {
		int keepcnt = 5;			//The maximum number of keepalive probes TCP should send before dropping the connection
		int keepidle = 10;		// The time (in seconds) the connection needs to remain idle before TCP starts sending keepalive probes
		int keepintvl = 30;		//The time (in seconds) between individual keepalive probes

        clients[i] = new WiFiClient(newClient);
		clients[i]->setOption(TCP_KEEPCNT, &keepcnt);
		clients[i]->setOption(TCP_KEEPIDLE, &keepidle);
		clients[i]->setOption(TCP_KEEPINTVL, &keepintvl);
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
  for (int i=0 ; i<max_connections ; i++) 
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
    if ((iclient > max_connections) || (clients[iclient] == NULL))
        return 0;
    size_t ret_val = clients[iclient]->write(buffer, size);
    return ret_val;
}

void Telnet::print(int iclient , String string)
{
    if ((iclient > max_connections) || (clients[iclient] == NULL))
	    return;
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
 
// TelNetQueue implementation
// 
//

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
	char str[NET_BUFSIZE + 1];

	memset(str, 0, (NET_BUFSIZE + 1) * sizeof(char));
	strncpy(str, (char*)buffer, min((size_t)NET_BUFSIZE, size));
	str[NET_BUFSIZE - 1] = '\0';

	xQueueSend(write_queue, str, 0);
}
size_t TelnetQueue::readAll(const uint8_t* buffer, size_t size)
{
	char str[NET_BUFSIZE + 1];
	memset(str, 0, (NET_BUFSIZE + 1) * sizeof(char));
	if (xQueueReceive(read_queue, str, 0))
	{
		strcpy((char*)buffer, str);
		DebugServer.writeAll(buffer, (size_t)strlen((char*)buffer));
		return strlen(str);
	}
	return 0;
}


// TelnetClient implementation
//

void TelnetClient::begin(WiFiClient *client)
{
  pclient = client;
  if (pclient != NULL)
  {
      pclient->setNoDelay(true);
  }
} 

void TelnetClient::handle()
{
	CheckForDroppedConnections();
}

uint8_t TelnetClient::available()
{
 if (pclient != NULL)
    {    
    return pclient->available();    
    }  
}


size_t TelnetClient::read(const uint8_t *buffer, size_t size)
{
 if (pclient != NULL)
    {
    if (pclient->connected() && pclient->available())
      {
      return pclient->read((uint8_t  *)buffer, size);  
      }  
    } 
}

size_t TelnetClient::write(const uint8_t *buffer, size_t size)
{
    if (pclient != NULL && pclient->connected())
    {
        size_t ret_val = pclient->write(buffer, size);
        return ret_val;
    }
    return 0;
}

uint8_t TelnetClient::connected()
{
  if (pclient != NULL)
    {   
    return pclient->connected(); 
    }
  return 0;
}


void TelnetClient::CheckForDroppedConnections()
{
  if (NULL != pclient)
  {
    if (pclient->connected() == false)
    {
      Serial.println("remove client");
      pclient->stop();
      pclient = NULL;
    }
  }
}

//	TelnetClientQueue
//  Create queues for sending and receiving data to VFO using TRXNET
//  Write puts a string in the queue to be picked up by task1_client to send it
//  In this way the main loop is not blocked, if queue if full message is dropped


void TelnetClientQueue::begin()
{
    if (write_queue == NULL)
    {
	    write_queue = xQueueCreate(10, NET_BUFSIZE * sizeof(char));
	    if (write_queue == NULL) {
		    Serial.println("Error creating the queue");
	    }
    }

    if (read_queue == NULL)
    {
        read_queue = xQueueCreate(10, NET_BUFSIZE * sizeof(char));
        if (read_queue == NULL) {
            Serial.println("Error creating the queue");
        }
    }
}

uint8_t TelnetClientQueue::connected()
{
    if (TRXNETClient.connected())
        return 1;
    else
        return 0;
}

size_t TelnetClientQueue::write(const uint8_t* buffer, size_t size)
{
	char str[NET_BUFSIZE + 1];

	memset(str, 0, (NET_BUFSIZE + 1) * sizeof(char));
	strncpy(str, (char*)buffer, min((size_t)NET_BUFSIZE, size));
    str[NET_BUFSIZE - 1] = '\0';
	xQueueSend(write_queue, str, 0);
}
size_t TelnetClientQueue::read(const uint8_t* buffer, size_t size)
{
	char str[NET_BUFSIZE + 1];
	memset(str, 0, (NET_BUFSIZE + 1) * sizeof(char));
	if (xQueueReceive(read_queue, str, 0))
	{
		strcpy((char*)buffer, str);
		return strlen(str);
	}
	return 0;
}