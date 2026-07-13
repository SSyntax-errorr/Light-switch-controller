#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>

const char* ssid = "Telecom-vmL6";
const char* password = "j89xUchM";

const char* mqtt_server = "192.168.100.233"; 
// Example: 192.168.1.50

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

  if (message == "ON=" || message == "ON") {
    myservo.write(0);
    lightOn = true;
  } else if (message == "OFF=" || message == "OFF")
  {
    myservo.write(90);
    lightOn = false;
  }
}


void reconnect() {

  while (!client.connected()) {

    Serial.print("Connecting MQTT...");

    if (client.connect("ESP32-WallE")) {

      Serial.println("connected");

      client.subscribe("home/command");

    } else {

      Serial.print("failed: ");
      Serial.println(client.state());

      delay(3000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BTN_PIN, INPUT_PULLUP);

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

  myservo.attach(SERVO_PIN);
  myservo.write(0);
  lightOn = true;
  Serial.println("\nWiFi connected");
  Serial.println(WiFi.localIP());

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}


void moveServo(){

  if (lightOn){
    myservo.write(90);
    lightOn = false;
  } else if (!lightOn) {
    myservo.write(0);
    lightOn = true;
  } else {
    Serial.print("fuck you");
  }

}


bool lastButtonState = HIGH;

void loop() {

  bool currentButtonState = digitalRead(BTN_PIN);

  if (currentButtonState == LOW && lastButtonState == HIGH) {
    Serial.print("fuck you!");
    moveServo();
    delay(50); 
  }

  lastButtonState = currentButtonState;

  if (!client.connected()) {
    reconnect();
  }

  client.loop();
}