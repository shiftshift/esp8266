#include <ESP8266WiFi.h> 
const char* ssid = "ssid";
const char* password = "password";
 
int ledPin = 2;         //gpio2(edit it)
WiFiServer server(80);  //initlialize with http server
 
void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(ledPin, OUTPUT);

  Serial.println("");
  Serial.print("Connecting to ");
  Serial.println(ssid);   //print and add '\n'
  WiFi.begin(ssid, password); //Initializes the WiFi library's network settings and provides the current status.
 
  while (WiFi.status() != WL_CONNECTED) { //until connect to a network
    delay(500);   //0.5s
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();   //http server begin
  Serial.println("Server started");
 
  Serial.print("Use this URL to connect: http://"); //Configures the specified pin to behave either as an input or an output
  Serial.print(WiFi.localIP());
}
 
void loop() {
  WiFiClient client = server.available(); //Gets a client that is connected to the server and has data available for reading
  if (!client) {  //no data,no client
    return;
  }

  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  String request = client.readStringUntil('\r');//get first line data
  Serial.println(request);
  client.flush();
 
  int value = LOW;
  if (request.indexOf("/LED=ON") != -1)  {  //Locates "/LED=ON" within request
    digitalWrite(ledPin, LOW);
    value = LOW;
  }
  if (request.indexOf("/LED=OFF") != -1)  {
    digitalWrite(ledPin, HIGH);
    value = HIGH;
  }
  
  //print html text
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
 
  client.print("Led is now: ");
 
  if(value == LOW) {
    client.print("On");
  } else {
    client.print("Off");
  }
  client.println("<br><br>");
  client.println("<a href=\"/LED=ON\"\"><button>On </button></a>");
  client.println("<a href=\"/LED=OFF\"\"><button>Off </button></a><br />");  
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
 
}
