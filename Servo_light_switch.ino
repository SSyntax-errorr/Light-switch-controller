#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>
#include <PubSubClient.h>
#include <Servo.h>
#include "secrets.h"
#include "update.h"

const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// const char* server_ip = SERVER_IP;

const char* mqtt_server = MQTT_SERVER; 

constexpr int LIGHT_ON_ANGLE = 0;
constexpr int LIGHT_OFF_ANGLE = 90;
constexpr int SERVO_MOVE_TIME = 500;



WiFiClient espClient;
PubSubClient client(espClient);

const int SERVO_PIN = D1;

Servo myservo;

bool lightOn = true;

const int BTN_PIN = D3;





void callback(char* topic, byte* payload, unsigned int length) {

  String message = "";

  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.print("Received: ");
  Serial.println(message);
  blink();
  if (message == "ON=" || message == "ON") {
    myservo.attach(SERVO_PIN);
    myservo.write(LIGHT_ON_ANGLE);
    lightOn = true;
    delay(500); 
    myservo.detach();

  } else if (message == "OFF=" || message == "OFF")
  {
    myservo.attach(SERVO_PIN);
    myservo.write(LIGHT_OFF_ANGLE);
    lightOn = false;
    delay(500); 
    myservo.detach();

  }
}


void reconnect() {

  while (!client.connected()) {

    Serial.print("Connecting MQTT...");

    if (client.connect("SmartRoom-Light")) {

      Serial.println("connected");

      client.subscribe("home/command");


      //LED to confirm setup is complete
      for (int i = 0; i < 4; i++){
        digitalWrite(LED_BUILTIN, LOW);   // ON
        delay(100);

        digitalWrite(LED_BUILTIN, HIGH);  // OFF
        delay(100);
      }

    } else {

      Serial.print("failed: ");
      Serial.println(client.state());

      delay(3000);
    }
  }
}



void setup() {
  Serial.begin(115200);
  // Serial.print("Update was successful!!!");
  pinMode(BTN_PIN, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  // attachInterrupt(BTN_PIN, moveServo, LOW);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(true);
  delay(1000);

  WiFi.begin(ssid, password);

  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 30) {
    delay(500);
    Serial.printf("Status: %d\n", WiFi.status());
    tries++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi failed, restarting...");
    ESP.restart();
  }


  checkForUpdates();


  myservo.attach(SERVO_PIN);
  myservo.write(LIGHT_ON_ANGLE);
  lightOn = true;
  delay(500); 
  myservo.detach();

  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);




  // moveServoTest();
}


void moveServoTest() {
    myservo.attach(SERVO_PIN);
    Serial.print("Testing servo!!");
    while (true) {
        myservo.write(0);
        delay(1000);

        myservo.write(90);
        delay(1000);
    }
}

void moveServo(){

  if (lightOn){
    myservo.attach(SERVO_PIN);
    delay(50);
    myservo.write(LIGHT_OFF_ANGLE);
    delay(500);
    myservo.detach();
    lightOn = false;
  } else if (!lightOn) {
    myservo.attach(SERVO_PIN);
    myservo.write(LIGHT_ON_ANGLE);
    delay(500); 
    myservo.detach();
    lightOn = true;
  } else {
    Serial.print("If this line somehow was reached just kys at this point.");
  }

}


bool lastButtonState = HIGH;
bool checked = false;


void blink() {
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
}


void loop() {

  bool currentButtonState = digitalRead(BTN_PIN);
  if (currentButtonState == LOW ){
    Serial.print("xxxx");
  }

  
  if (currentButtonState == LOW && lastButtonState == HIGH) {
    Serial.print("fuck you!");
    delay(100); 
    blink();
    blink();
    blink();
    moveServo();
    
  } else {
    // Serial.print("xxx");
  }

  lastButtonState = currentButtonState;



  if (!client.connected()) {
    reconnect();
  }



  client.loop();
}