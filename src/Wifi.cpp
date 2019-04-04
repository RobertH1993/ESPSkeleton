#include "Wifi.h"
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

#define WIFI_LED_PIN 2
#define NTP_PACKET_SIZE 48
uint8_t led_state = 0;
uint64_t blink_time = 0;

//Globals
WiFiUDP udp;

//Public
uint8_t wifi_init_ap_mode(char *ssid, char *password){
  return WiFi.softAP(ssid, password);
}

//Public
uint8_t wifi_ap_mode_get_number_of_connected_clients(){
  return WiFi.softAPgetStationNum();
}

//Public
uint8_t wifi_stop_ap_mode(){
  return WiFi.softAPdisconnect(true);
}

//Public
void wifi_set_static_configuration(uint8_t* ip_address, uint8_t* netmask, uint8_t* gateway, uint8_t* dns_server1, uint8_t* dns_server2){
  WiFi.config(IPAddress(
                        ip_address[0],
                        ip_address[1],
                        ip_address[2],
                        ip_address[3]
                        ),
               IPAddress(
                          gateway[0],
                          gateway[1],
                          gateway[2],
                          gateway[3]
                         ),
               IPAddress(
                          netmask[0],
                          netmask[1],
                          netmask[2],
                          netmask[3]
                        ),
               IPAddress(
                          dns_server1[0],
                          dns_server1[1],
                          dns_server1[2],
                          dns_server1[3]
                        ),
               IPAddress(
                          dns_server2[0],
                          dns_server2[1],
                          dns_server2[2],
                          dns_server2[3]
                        )
    );
}

//Public
void wifi_connect_to_ap(char *ssid, char *password){
  WiFi.begin(ssid, password);
  WiFi.setAutoReconnect(false);
}

//Public
void wifi_stop_sta_mode(){
  WiFi.disconnect(true);
}

//Public
uint8_t wifi_is_connected_to_ap(){
  return WiFi.status() == WL_CONNECTED;
}

//Public
void wifi_set_status_led(uint8_t state){
  pinMode(WIFI_LED_PIN, OUTPUT);
  if(state == 1){
    digitalWrite(WIFI_LED_PIN, LOW);
  } else {
    digitalWrite(WIFI_LED_PIN, HIGH);
  }
  led_state = state;
}

//Public
void wifi_blink_status_led(uint16_t delay_miliseconds){
  if((millis() - blink_time) >= delay_miliseconds){
    if(led_state == 0) wifi_set_status_led(1);
    else wifi_set_status_led(0);

    blink_time = millis();
  }
}

//Public
void wifi_send_NTP_request(uint8_t* ntp_server){
  IPAddress server_ip(ntp_server[0], ntp_server[1], ntp_server[2], ntp_server[3]);
  uint8_t ntp_packet[NTP_PACKET_SIZE] = {};
  ntp_packet[0] = 0b11100011;  //LI, Version, Mode
  ntp_packet[1] = 0; //Stratum, or type of clock
  ntp_packet[2] = 6; //Polling interval
  ntp_packet[3] = 0xEC; //Peer Clock Precision
  ntp_packet[12] = 49;
  ntp_packet[13] = 0x4E;
  ntp_packet[14] = 49;
  ntp_packet[15] = 52;

  //Send packet multiple times because its UDP
  udp.begin(2390);
  for(uint8_t i = 0; i < 5; i++){
    udp.beginPacket(server_ip, 123);
    udp.write(ntp_packet, NTP_PACKET_SIZE);
    udp.endPacket();
  }
}

//Public
uint32_t wifi_get_NTP_response(){
  if(udp.parsePacket() == 0) return 0; //No packet received

  uint8_t ntp_packet[NTP_PACKET_SIZE] {};
  udp.read(ntp_packet, NTP_PACKET_SIZE);
  udp.stop();

  uint32_t highWord = word(ntp_packet[40], ntp_packet[41]);
  uint32_t lowWord = word(ntp_packet[42], ntp_packet[43]);
  uint32_t ntpTime = highWord << 16 | lowWord;
  return ntpTime - 2208988800UL;
}
