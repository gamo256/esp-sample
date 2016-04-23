int sensorPin = 0;
int errorLED = 11;
String ssid     = "<SSID>";  // wifi SSID
String password = "<Password>"; // wifi Password
String host     = "dweet.io"; // dweet.io
const int httpPort   = 80;
String uri     = "/dweet/for/arduino-temperature";
// the setup routine runs once when you press reset
void setup() {
  pinMode(errorLED, OUTPUT); // init our red error LED
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(5000);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) digitalWrite(errorLED, HIGH); // check if the ESP is running well
  // Connect to Wifi
  Serial.println("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"");
  delay(10000);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) digitalWrite(errorLED, HIGH); // check if the ESP is running well
  // Open TCP connection to the host:
  Serial.println("AT+CIPSTART=\"TCP\",\"" + host + "\"," + httpPort);
  delay(50);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) digitalWrite(errorLED, HIGH); // check if the ESP is running well
}
// the loop routine runs over and over again forever:
void loop() {
  int reading = analogRead(sensorPin);
  float voltage = reading * 5.0 / 1024.0;
  float temperatureC = voltage * 100;
  // Construct our HTTP call
  String httpPacket = "POST " + uri +"?temperature="+ temperatureC + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  // Send our message length
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond
  if (!Serial.find(">")) digitalWrite(errorLED, HIGH); // check if the ESP is running well
  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  if (!Serial.find("SEND OK\r\n")) digitalWrite(errorLED, HIGH); // check if the ESP is running well
  delay(10000);  // wait 10 seconds before updating
}
