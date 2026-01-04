#include <Arduino.h>

//TEXAS STATE LAB
//Created by Stephan Cope with code examples from Arduino
//2-12-25
//
//This code is ONLY compatible with MOSAIC when the ESP32 board is plugged in 
//AND you are using the Gamma Ray Payload
//
//This example waits for the Gamma Ray detector to pickup a pulse. 
//That pulse is sent to the ESP32 ADC
//All data into the ADC is then converted into digital serial outputs that can be visualized

//int analogPin = 26;

int ledPin = 2;

void setup() {

  pinMode(ledPin, OUTPUT);
  // initialize serial communication at 115200 bits per second:
  Serial.begin(115200);

  //set the resolution to 12 bits (0-4095)
  analogReadResolution(12);
}

void loop() {
  // Check for incoming command
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim(); // Remove any whitespace/newlines
    if (command != "") {
      //digitalWrite(ledPin, HIGH);
      //delay(500);
      digitalWrite(ledPin, LOW);
      delay(500);
    }
    if (command == "gamma_begin") {
      Serial.printf("Command Received");
      digitalWrite(ledPin, HIGH);
      delay(1000);
      int analogPin = 33;
      int i = 0;
      int tot_Volts = 0;

      while (i < 100) {
        int analogVolts = analogReadMilliVolts(analogPin);
        tot_Volts = tot_Volts + analogVolts;
        i = i + 1;
      }
      float avg_Volts = static_cast<float>(tot_Volts) / i;
      
      // Format data
      char dataBuffer[32];
      int dataLen = snprintf(dataBuffer, sizeof(dataBuffer), "%.2f\n", avg_Volts);
      
      // Send Protocol Header: <GAMMA_START><SIZE>[4-byte uint32]</SIZE>
      Serial.print("<GAMMA_START><SIZE>");
      
      uint32_t size = (uint32_t)dataLen;
      Serial.write((uint8_t*)&size, sizeof(size));
      
      Serial.print("</SIZE>");
      
      // Send Data
      Serial.write((uint8_t*)dataBuffer, dataLen);
      
      // Send Footer
      Serial.print("<GAMMA_END>");
    }
  }
}