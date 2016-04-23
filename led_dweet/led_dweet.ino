int LED = 5;
int errorLED = 11;
String ssid     = "<SSID>";  // wifi SSID
String password = "<Password>"; // wifi password
String host     = "dweet.io"; // dweet.io
String thingname     = "arduino-led-onoff"; // thing name
const int httpPort   = 80;
String uri     = "/get/latest/dweet/for/" + thingname;
// the setup routine runs once when you press reset:
void setup() {
  pinMode(LED, OUTPUT); // init our bule LED
  pinMode(errorLED, OUTPUT); // init our red error LED
  // Start our ESP8266 Serial Communication
  Serial.begin(115200);   // Serial connection over USB to computer
  Serial.println("AT");   // Serial connection on Tx / Rx port to ESP8266
  delay(5000);        // Wait a little for the ESP to respond
  if (!Serial.find("OK")) digitalWrite(errorLED, HIGH); // check if the ESP is running wel
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
  // Construct our HTTP call
  String httpPacket = "GET " + uri + " HTTP/1.1\r\nHost: " + host + "\r\n\r\n";
  int length = httpPacket.length();
  // Send our message leuntilngth
  Serial.print("AT+CIPSEND=");
  Serial.println(length);
  delay(10); // Wait a little for the ESP to respond
  if (!Serial.find(">")) {
    digitalWrite(errorLED, HIGH); // check if the ESP is running well
  } else {
    digitalWrite(errorLED, LOW);
  }
  // Send our http request
  Serial.print(httpPacket);
  delay(10); // Wait a little for the ESP to respond
  if (!Serial.find("SEND OK\r\n")) digitalWrite(errorLED, HIGH); // check if the ESP is running well
  while(!Serial.available()) delay(5);  // wait  we receive the response from the server
  if (Serial.find("\r\n")){ // search for a blank line which defines the end of the http header
    delay(5);
    unsigned int i = 0; //timeout counter
    String outputString = "";
    while (!Serial.find("\"action\":\"")){} // find the part we are interested in.
    while (i<60000) { // 1 minute timeout checker
      if(Serial.available()) {
        char c = Serial.read();
        if(c=='\"') break; // break out of our loop because we got all we need
        outputString += c; // append to our output string
        i=0; // reset our timeout counter
      }
      i++;
    }
    if(outputString=="on"){
      digitalWrite(LED, HIGH);
    } else {
      digitalWrite(LED, LOW);
    }
  }
  delay(10000); // wait 10 seconds before updating
}
