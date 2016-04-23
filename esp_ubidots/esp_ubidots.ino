#include <ArduinoJson.h>
int sensorPin = 0;
int ERRLED =11;
String SSID = "<SSID>"
String PASS = "<Password>";
String TOKEN =  "<Token>";
String SERVER = "things.ubidots.com";
int PORT = 80;
String ID = "<Variable ID>";
String uri= "/api/v1.6/variables/" + ID + "/values";
void setup() {
  pinMode(ERRLED, OUTPUT);
  Serial.begin(115200);
  Serial.println("AT");
  if (!Serial.find("OK")) digitalWrite(ERRLED, HIGH);
  // Connect Wifi
  Serial.println("AT+CWJAP=\"" + SSID + "\",\"" + PASS + "\"");
  delay(10000);
  if (!Serial.find("OK")) digitalWrite(ERRLED, HIGH);
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + SERVER + "\"," + PORT);
  delay(50);
  if (!Serial.find("OK")) digitalWrite(ERRLED, HIGH);
}
void loop() {
  int reading = analogRead(sensorPin);
  float voltage = reading * 5.0 / 1024.0;
  float temperatureC = voltage * 100;
  // Reserve memory space
  StaticJsonBuffer<200> jsonBuffer;
  // Build object tree in memory
  JsonObject& dat = jsonBuffer.createObject();
  dat["value"] =  temperatureC;
  String value;
  dat.printTo(value);
  value += "\r\n";
  int contentlength = value.length();
  // Construct our HTTP call
  String httpPacket = "POST " + uri + " HTTP/1.1\r\nHost: " + SERVER + "\r\nX-Auth-Token: "+ TOKEN +"\r\nContent-Length: " + contentlength + " \r\nContent-Type: application/json\r\n\r\n" + value +"\r\n";
  int length = httpPacket.length();
// Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond
  // check if the ESP is running well
  if (!Serial.find(">")) digitalWrite(ERRLED, HIGH);
  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  // check if the ESP is running wel
  if (!Serial.find("SEND OK\r\n")) digitalWrite(ERRLED, HIGH);
  delay(10000);  // wait 10 seconds before updating
}
