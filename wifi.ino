#include <ESP8266WiFi.h>

const char* ssid = "Galaxy A312E61";           // Your WiFi SSID
const char* password = "vuxm72523";            // Your WiFi Password

WiFiServer server(80);                         // Start server on port 80
String ledStatus = "OFF";                      // To hold LED state
String buttonStatus = "RELEASED";              // To hold button state

void setup() {
  Serial.begin(9600);                          // Initialize Serial connection to Arduino
  WiFi.begin(ssid, password);                  // Connect to WiFi

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();                              // Start server
}

void loop() {
  // Read LED and button states from Arduino over Serial
  if (Serial.available() > 0) {
    String received = Serial.readStringUntil('\n');  // Read the Serial input
    if (received.startsWith("LED:")) {
      int ledPos = received.indexOf("LED:") + 4;
      int btnPos = received.indexOf(", Button:") + 10;
      ledStatus = received.substring(ledPos, received.indexOf(",", ledPos));
      buttonStatus = received.substring(btnPos);
    }
  }

  WiFiClient client = server.available();      // Wait for client

  if (client) {
    String request = client.readStringUntil('\r');
    client.flush();

    // Send HTML response with LED and button states and refresh meta tag
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    client.println("<head><meta http-equiv=\"refresh\" content=\"1\"></head>");
    client.println("<body>");
    client.println("<h1>ESP8266 LED and Button Control</h1>");
    client.println("<p>LED State: " + ledStatus + "</p>");
    client.println("<p>Button State: " + buttonStatus + "</p>");
    client.println("</body>");
    client.println("</html>");
    
    client.stop();
    Serial.println("Client Disconnected.");
  }
}
