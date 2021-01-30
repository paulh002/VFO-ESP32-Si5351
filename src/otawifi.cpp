
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
#include "Fonts/DroidSansMono8pt7b.h"
#include "Fonts/DroidSansMono12pt7b.h"
#include "Fonts/DroidSansMono24pt7b.h"
#include "Fonts/DroidSansMono48pt7b.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Update.h>



const char* host = "esp32";
#define NUMBR_SSID  2
const char* ssid[] = {"wifi", "password" };
const char* password[] = {"wifi", "password"};
extern  Adafruit_ILI9341 tft;

WebServer server(80);


/*
 * Server Index Page
 */
 
const char* serverIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='#' enctype='multipart/form-data' id='upload_form'>"
   "<input type='file' name='update'>"
        "<input type='submit' value='Update'>"
    "</form>"
 "<div id='prg'>progress: 0%</div>"
 "<script>"
  "$('form').submit(function(e){"
  "e.preventDefault();"
  "var form = $('#upload_form')[0];"
  "var data = new FormData(form);"
  " $.ajax({"
  "url: '/update',"
  "type: 'POST',"
  "data: data,"
  "contentType: false,"
  "processData:false,"
  "xhr: function() {"
  "var xhr = new window.XMLHttpRequest();"
  "xhr.upload.addEventListener('progress', function(evt) {"
  "if (evt.lengthComputable) {"
  "var per = evt.loaded / evt.total;"
  "$('#prg').html('progress: ' + Math.round(per*100) + '%');"
  "}"
  "}, false);"
  "return xhr;"
  "},"
  "success:function(d, s) {"
  "console.log('success!')" 
 "},"
 "error: function (a, b, c) {"
 "}"
 "});"
 "});"
 "</script>";

/*
 * setup function
 */
void otasetup(void) {
  
  char str[80], str1[80];
  int timer = 0;
  int ssid_count = 0;
  
  // Connect to WiFi network
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
   -       tft.print("Try to connect to: ");
          tft.println(ssid[ssid_count]);
          WiFi.begin(ssid[ssid_count], password[ssid_count]);
        }
       else if (ssid_count == NUMBR_SSID)
       {
          WiFi.disconnect();
          ESP.restart();
       }
  }
  
  String ip = WiFi.localIP().toString();
  tft.println("");
  ip.toCharArray(str1, 80);
  sprintf(str,"Connected to %s ", ssid[ssid_count]);
  tft.println(str);
  sprintf(str,"IP address:  %s", str1);
  tft.println(str);
  
  /*use mdns for host name resolution*/
  if (!MDNS.begin(host)) { //http://esp32.local
    Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }
  Serial.println("mDNS responder started");
  /*return index page which is stored in serverIndex */
  server.on("/", HTTP_GET, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", serverIndex);
  });
  /*handling uploading firmware file */
  server.on("/update", HTTP_POST, []() {
    server.sendHeader("Connection", "close");
    server.send(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    ESP.restart();
  }, []() {
    HTTPUpload& upload = server.upload();
    if (upload.status == UPLOAD_FILE_START) {
        char str[80];
        
        sprintf(str,"Update: %s\n", upload.filename.c_str());
        tft.println(str);
      
      Serial.printf("Update: %s\n", upload.filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_WRITE) {
      /* flashing firmware to ESP*/
      if (Update.write(upload.buf, upload.currentSize) != upload.currentSize) {
        Update.printError(Serial);
      }
    } else if (upload.status == UPLOAD_FILE_END) {
      if (Update.end(true)) { //true to set the size to the current progress     
        char str[80];
        sprintf(str, "Update Success: %u\nRebooting...\n", upload.totalSize);
        tft.println(str);
        delay(500);
        
      } else {
        Update.printError(Serial);
      }
    }
  });
  server.begin();
}


void otawifi()
{
  tft.fillScreen(ILI9341_BLACK);
  tft.setFont(&DroidSansMono8pt7b);
  tft.setCursor(0, 20);
  tft.print("Start OTA WIFI");
  
  otasetup();
  
  while(1)
  {
      server.handleClient();
      delay(1);
  }
}
