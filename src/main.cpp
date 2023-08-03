#include <Arduino.h>
#include <SoftwareSerial.h>
#include "../lib/EBYTE/EBYTE.h"
#include "../lib/ArduinoJson/ArduinoJson.h"

#define PIN_RX 14   //D5 on the board (Connect this to the EBYTE TX pin)
#define PIN_TX 12   //D6 on the board (connect this to the EBYTE RX pin)

#define PIN_M0 5    //D1 on the board (connect this to the EBYTE M0 pin)
#define PIN_M1 4    //D2 on the board (connect this to the EBYTE M1 pin)
#define PIN_AX 16   //D0 on the board (connect this to the EBYTE AUX pin)

String name = "Anonymous";

// you will need to define the pins to create the serial port
SoftwareSerial ESerial(PIN_RX, PIN_TX);

// create the transceiver object, passing in the serial and pins
EBYTE Transceiver(&ESerial, PIN_M0, PIN_M1, PIN_AX);

DynamicJsonBuffer jBuffer;
JsonObject &OutgoingObj = jBuffer.createObject();

void setup() {
    Serial.begin(9600);

    ESerial.begin(9600);

    Serial.println("Starting Chat");
    Serial.println(Transceiver.init());
    Transceiver.PrintParameters();

    Serial.println("Set your name:");
    while (!Serial.available()) {
        delay(10);
    }
    name = Serial.readString();
    Serial.print("Name set to:");
    Serial.println(name);
}
void PrintMessage(JsonObject *msg){
    Serial.print((msg->get<String>("name")));
    Serial.print(": ");
    Serial.println((msg->get<String>("msg")));
}
void loop() {
    if (ESerial.available()) {
        String input = ESerial.readString();
        JsonObject &IncommingObj = jBuffer.parseObject(input);
        PrintMessage(&IncommingObj);
    }
    if (Serial.available()) {
        OutgoingObj["name"] = name;
        OutgoingObj["msg"] = Serial.readString();
        String output;
        OutgoingObj.printTo(output);
        ESerial.print(output);
        PrintMessage(&OutgoingObj);
    }
}

