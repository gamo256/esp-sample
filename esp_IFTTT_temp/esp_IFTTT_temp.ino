#include <ArduinoJson.h>
int sensorPin = 0;
int errorLED = 11;
String ssid     = "<SSID>";  // wifi SSID
String password = "<Password>"; // wifi password
String host     = "maker.ifttt.com";
const int httpPort   = 80;
String key = "<key>"; // Your IFTTT Maker key
String event = "<EventName>"; // Your IFTTT event name
String uri     = "/trigger/" + event + "/with/key/" + key;
void setup() {
  pinMode(errorLED, OUTPUT);
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);
  Serial.println("AT");
  delay(5000);
  if (!Serial.find("OK")) digitalWrite(errorLED, HIGH);
  // Connect to Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10000);
  if (!Serial.find("OK")) digitalWrite(errorLED, HIGH);
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);
  if (!Serial.find("OK")) digitalWrite(errorLED, HIGH);
}
void loop() {
  int reading = analogRead(sensorPin);
  float voltage = reading * 5.0 / 1024.0;
  float temperatureC = voltage * 100;
  // Reserve memory space
  StaticJsonBuffer<200> jsonBuffer;
  // Build object tree in memory
  JsonObject& dat = jsonBuffer.createObject();
  // set data to sending
  dat["value1"] =  temperatureC;;
  // set opitinal data to sending
  // dat["value2"] =  value; // optional
  // dat["value3"] =  value; // optional
  String value;
  dat.printTo(value);
  value += "\r\n";
  int contentlength = value.length();
  // Construct our HTTP call
  String httpPacket = "POST " + uri + " HTTP/1.1\r\nHost: " + host + "\r\nContent-Length: " + contentlength + " \r\nContent-Type: application/json\r\n\r\n" + value +"\r\n";
  int length = httpPacket.length();
// Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10);
  if (!Serial.find(">")) digitalWrite(errorLED, HIGH);
  // Send our http request
  Serial.print(httpPacket);
  delay(10);
  if (!Serial.find("SEND OK\r\n")) digitalWrite(errorLED, HIGH);
  delay(10000);
}
