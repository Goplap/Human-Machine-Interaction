const int buttonPin = 4;      // Pin for the button
const int ledPin = 3;         // Pin for the LED
bool ledState = false;        // Tracks the LED state
bool lastButtonState = LOW;   // Tracks the last button state

void setup() {
  pinMode(buttonPin, INPUT);   // Set button pin as input
  pinMode(ledPin, OUTPUT);     // Set LED pin as output
  Serial.begin(9600);
}

void loop() {
  // Read the button state
  bool currentButtonState = digitalRead(buttonPin);

  // Toggle LED state on button press
  if (currentButtonState && !lastButtonState) {  // Check for button press
    ledState = !ledState;                        // Toggle LED state
    digitalWrite(ledPin, ledState ? HIGH : LOW); // Update LED output
  }

  // Update last button state
  lastButtonState = currentButtonState;

  // Send LED and button states to ESP8266
  Serial.print("LED:");
  Serial.print(ledState ? "ON" : "OFF");
  Serial.print(",Button:");
  Serial.println(currentButtonState ? "PRESSED" : "RELEASED");

  delay(50);  // Small delay to debounce
}
