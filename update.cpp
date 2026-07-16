#include "update.h"

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

#include "secrets.h"

static const char* server = MQTT_SERVER;
const String currentVersion = "0.8";

String buildURL(const char* endpoint) {
  return String("http://") + server + ":8080/" + endpoint;
}





String getLatestVersion(){

  WiFiClient client;
  HTTPClient http;

  http.begin(client, buildURL("version"));

  int code = http.GET();

  String ver = "";

  if(code == HTTP_CODE_OK){
    ver = http.getString();
  }

  http.end();

  return ver;
}




void updateFirmware() {
  Serial.print("update firmware reached!");
  WiFiClient client;

  String url = "http://192.168.100.233:8080/firmware";

  t_httpUpdate_return ret = ESPhttpUpdate.update(client, url);

  switch(ret) {

    case HTTP_UPDATE_FAILED:
      Serial.println("Update failed");
      Serial.println(ESPhttpUpdate.getLastErrorString());
      break;

    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("Already latest version");
      break;

    case HTTP_UPDATE_OK:
      Serial.println("Update successful");
      break;
  }
}





void checkForUpdates(){



      //Check server version and update if needed
    String latestVersion = getLatestVersion();
    latestVersion.trim();
    // Serial.print(latestVersion);
    if (latestVersion != currentVersion) {
      Serial.println(latestVersion);
      Serial.print(currentVersion);
      updateFirmware();
      Serial.print("Version updated succesfully!");

    } else if (latestVersion == currentVersion)
    {
      Serial.print("Already at latest Version!");
    }
}

