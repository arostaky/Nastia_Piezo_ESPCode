/*---------------------------------------------------------------------------------------------

  Open Sound Control (OSC) library for the ESP8266

  Example for sending messages from the ESP8266 to a remote computer
  The example is sending "hello, osc." to the address "/test".

  This example code is in the public domain.

  --------------------------------------------------------------------------------------------- */
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>

char ssid[] = "didanet-blanc";          // your network SSID (name)
char pass[] = "";                    // your network password

WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192, 168, 0, 130);     // remote IP of your computer
const IPAddress local_IP(192, 168, 0, 100);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 0, 0);
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

//adding btn
const int analogInPin = A0;    // ESP8266 Analog Pin ADC0 = A0
int sensorValue = 0; // value read from the piezo
const int ledPin =  LED_BUILTIN;      // the number of the LED pin
float piezoV = 0;
void setup() {
  // initialize the LED pin as an output:
//  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);

  // Connect to WiFi network
  Serial.println();
  Serial.println();
    // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");

  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Starting UDP");
  Udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(Udp.localPort());

}
void loop() {
 sensorValue = analogRead(analogInPin);
 Serial.print("piezo = ");
 piezoV = sensorValue / 1023.0 * 5.0;
 Serial.println(piezoV); // Print the voltage
 sendMsg();
 delay(100);
}
void sendMsg() {
  OSCMessage msg("/piezo");
  msg.add(piezoV);
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
}
