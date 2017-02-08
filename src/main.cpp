#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

#define MAX_SRV_CLIENTS 1

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
// telnet defaults to port 23
EthernetServer server = EthernetServer(23);

void setup() {
    Serial1.begin(115200);

    Serial1.print("\nConnecting to ");
    Serial.begin(115200);
    // initialize the ethernet device
    if(Ethernet.begin(mac))
        Serial.println(Ethernet.localIP());
    else
        Serial.println("falied");

    server.begin();
    Serial1.print("Ready! Use 'telnet ");
    Serial1.print(Ethernet.localIP());
    Serial1.println(" 23' to connect");
}

void loop() {
    Ethernet.maintain();
    EthernetClient client = server.available();
    if (client) {
        while(client.available()) Serial.write(client.read());

    }
    //check UART for data
    if(Serial.available()){
        size_t len = Serial.available();
        uint8_t sbuf[len];
        Serial.readBytes(sbuf, len);
        //push UART data to all connected telnet clients
        if (client.connected()){
            client.write(sbuf, len);
            delay(1);
        }

    }
}