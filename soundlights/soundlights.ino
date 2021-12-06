

/*
  Скетч разработан 30.11.2018 Wirekraken
*/


#include <FS.h>
//#define FASTLED_ALLOW_INTERRUPTS 0
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include "LEDStrip.h"
#include "inlinetimer.h" 

const char* ssid = ""; // имя вашей сети
const char* password = ""; // пароль вашей сети

IPAddress Ip(192,168,2,45); // IP-адрес для ESP
IPAddress Gateway(192,168,2,254); // IP-адрес шлюза (роутера)
IPAddress Subnet(255,255,255,0); // маска подсети, диапазон IP-адресов в локальной сети


// инициализация websocket на 81 порту



LEDStrip ledstrip;
ESP8266WebServer server(80);

void setup(){

  Serial.begin(9600); 
 
  

  WiFi.config(Ip, Gateway, Subnet);
  WiFi.begin(ssid, password);
  

  while (WiFi.status() != WL_CONNECTED){ 
    delay(500);
    Serial.print(".");
  }
  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  
  server.begin();

 // Initialize SPIFFS
  if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  
  initServer();
ledstrip.init();
  
  Serial.println("Init complete...");  
}

void loop(){


  /*inlineTimer(5000, {
    if (WiFi.status() != WL_CONNECTED) { 
      Serial.println("Reconnecting Wifi");
      WiFi.reconnect();
    }
  });*/

  server.handleClient();
  ledstrip.process();

}

String getContentType(String filename){
    if(server.hasArg("download")) return "application/octet-stream";
    else if(filename.endsWith(".htm")) return "text/html";
    else if(filename.endsWith(".html")) return "text/html";
    else if(filename.endsWith(".css")) return "text/css";
    else if(filename.endsWith(".js")) return "application/javascript";
    else if(filename.endsWith(".png")) return "image/png";
    else if(filename.endsWith(".gif")) return "image/gif";
    else if(filename.endsWith(".jpg")) return "image/jpeg";
    else if(filename.endsWith(".ico")) return "image/x-icon";
    else if(filename.endsWith(".xml")) return "text/xml";
    else if(filename.endsWith(".pdf")) return "application/x-pdf";
    else if(filename.endsWith(".zip")) return "application/x-zip";
    else if(filename.endsWith(".gz")) return "application/x-gzip";
    return "text/plain";

}

bool handleFileRead(String path) { // send the right file to the client (if it exists)

  if (path.endsWith("/")) path += "index.html";         // If a folder is requested, send the index file
  String contentType = getContentType(path);            // Get the MIME type
  if (SPIFFS.exists(path)) {                            // If the file exists
    File file = SPIFFS.open(path, "r");                 // Open it
    size_t sent = server.streamFile(file, contentType); // And send it to the client
    file.close();                                       // Then close the file again
    return true;
  }
  Serial.println("\tFile Not Found");
  return false;                                         // If the file doesn't exist, return false
}
