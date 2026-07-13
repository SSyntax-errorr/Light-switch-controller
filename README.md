# Smart Room Light Switch

A personal IoT project that allows me to remotely turn my room's light switch on and off using a servo motor.

## Hardware

* Arduino D1 Mini (ESP8266)
* SG90 Servo Motor
* Push Button

## Software

* MQTT for receiving commands
* Spring Boot backend server
* Flutter mobile application

## Notes

### CH340 Driver Issue

Some cloned D1 Mini boards are incompatible with newer versions of the CH340 driver.

If the board is detected but fails to upload sketches, install **CH340 driver version 3.5.2019.1** instead of the latest release. 
Windows often auto updates drivers, so when communication fails, check driver version.
