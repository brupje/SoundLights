

class ConfigHandler: public RequestHandler {

public:
  void returnConfig(ESP8266WebServer& server, HTTPMethod requestMethod) {
    DynamicJsonDocument doc(1024);
    LEDStrip_config config = ledstrip.getConfig();    

    doc["mode"]=config.ledMode;
    doc["red"]=config.r;
    doc["blue"]=config.b;
    doc["green"]=config.g;
    doc["brightness"] = config.bright;
    doc["delay"] =config.delayValue;
    doc["color"]= String(config.r,HEX) +String(config.g,HEX) +String(config.b,HEX) ;
      
    if (requestMethod==HTTP_POST)
      doc["success"] = "true";  
      
    char buffer[1000];
    serializeJson(doc,buffer) ;
    server.send ( 200, "text/json",buffer);
  }


 String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}


  void handleConfigMain(ESP8266WebServer& server, HTTPMethod requestMethod) {
    Serial.println("Handle main config"); 
    DynamicJsonDocument doc(1024);
    LEDStrip_config config = ledstrip.getConfig();    
    if (requestMethod==HTTP_POST) {
      deserializeJson(doc, server.arg("plain"));
      JsonObject json= doc.as<JsonObject>();
      config.ledMode = json["mode"];
      config.r = json["red"];
      config.g = json["green"];
      config.b = json["blue"];
      config.bright =  json["brightness"];
      config.delayValue =  json["delay"];
      ledstrip.setConfig(config);  
    }

    returnConfig(server,requestMethod);
  
  }

  bool canHandle(HTTPMethod method, const String& uri) override { 
    return uri.startsWith( "/config/set" );
  }

  bool handle(ESP8266WebServer& server, HTTPMethod requestMethod, const String& requestUri) override{   
    Serial.println("Requested: "+requestUri); 
    if (requestUri.equals("/config/set")) {
      handleConfigMain(server,requestMethod);
      return true;
    }
    else {
      String suburl = requestUri.substring(12);
      Serial.println("Requested: "+suburl + " ("+ server.arg("plain")+")");
         
      if (requestMethod==HTTP_POST) {
        LEDStrip_config config = ledstrip.getConfig();    
        int retval =0;
        if (suburl.equals("brightness")) {
          config.bright = retval=  server.arg("plain").toInt()*2.55;
        }
        else if (suburl.equals("mode")) {
          config.ledMode = retval=  server.arg("plain").toInt();
        }
        else if (suburl.equals("color")) {
          
          config.r =getValue(server.arg("plain"), ',', 0).toInt();
          config.g =getValue(server.arg("plain"), ',', 1).toInt();
          config.b =getValue(server.arg("plain"), ',', 2).toInt();          
        }
        else {
          server.send ( 404,"text/plain", "Setting not available");
          return true;
        }
        ledstrip.setConfig(config);  

        returnConfig(server,requestMethod);
  
        //server.send ( 200,"text/plain", String(retval));

      }      
    }

    return true;
  }

} myHandler;


void initServer() {

  server.addHandler(&myHandler );

  server.onNotFound([]() {                              // If the client requests any URI
    if (!handleFileRead(server.uri()))                  // send it if it exists
      server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
  });

  

}