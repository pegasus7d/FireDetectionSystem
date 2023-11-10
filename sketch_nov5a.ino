// #include <ESP8266WiFi.h>
// #include <ESPAsyncTCP.h>
// #include <ESPAsyncWebServer.h>
// #include <Wire.h>
#include <ESPPubSubClientWrapper.h>

#define Buzzer D5
#define Green D6
#define Red D7
#define Sensor A0

// Replace with your network credentials
// #define wifi_ssid "prrockzed"
// #define wifi_password "Priyanshu123"
// #define mqtt_server "192.168.253.222"
#define smoke "sensor/reading"
const char *ssid = "debu";
const char *pass = "password";
uint8_t ip[4] = { 192, 168, 212, 222 };
ESPPubSubClientWrapper client(ip);

void setup_wifi() {
  delay(10);
  // int num_nets = WiFi.scanNetworks();

  // for(int i = 0; i < num_nets; ++i)
  // {
  //   Serial.printf("SSID: %s, signal: %i\n", WiFi.SSID(i).c_str(), WiFi.RSSI(i));
  // }


  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);
  WiFi.setSleep(false);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("!");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  // client.setServer(mqtt_server);

  pinMode(Green, OUTPUT);
  pinMode(Red, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(Sensor, INPUT);

  // WiFi.begin(wifi_ssid, wifi_password);
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Connecting to WiFi...");
  // }

  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());
}

// void reconnect() {
//   // Loop until we're reconnected
//   while (!client.connected()) {
//     Serial.print("Attempting MQTT connection...");

//     if (client.connect("ESP8266Client")) {
//       Serial.println("connected");
//     } else {
//       Serial.print("failed, rc=");
//       Serial.print(client.state());
//       Serial.println(" try again in 5 seconds");
//       delay(5000);
//     }
//   }
// }

void loop() {
  // if (!client.connected()) {
  //   reconnect();
  // }
  client.loop();

  // Wait a few seconds between measurements.
  delay(2000);

  int sensor = analogRead(Sensor);
  sensor = map(sensor, 0, 1023, 0, 100);

  if (sensor > 25) {
    digitalWrite(Green, LOW);
    digitalWrite(Red, HIGH);
    digitalWrite(Buzzer, HIGH);
    Serial.println("Warning! Gas leak detected. Sensor value: " + String(sensor));
    client.publish(smoke, ("Smoke Detected! Sensor value: " + String(sensor)).c_str());
  } else {
    digitalWrite(Green, HIGH);
    digitalWrite(Red, LOW);
    digitalWrite(Buzzer, LOW);
    Serial.println("Gas value: Normal. Sensor value: " + String(sensor));
    client.publish(smoke, ("No Smoke! Sensor value: " + String(sensor)).c_str());
  }

  delay(1000);  // Adjust the delay as needed
}