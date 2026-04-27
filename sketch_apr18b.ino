// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6)
#include <BluetoothSerial.h>

const int buttonPin = 4;   // GPIO pin connected to the button (other side to GND)
int buttonState = 0;       // Variable to store the read state

const int potPin = 34;
const int pwmPin = 16;
const int enPin = 18;

// Bluetooth object
BluetoothSerial SerialBT;

// variable for storing the potentiometer value
int potValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(pwmPin, OUTPUT);
  pinMode(enPin, OUTPUT);
   // Configure pin as input with internal pull-up enabled
  pinMode(buttonPin, INPUT_PULLUP);

  const int bt_on_button = 32;  // GPIO pin connected to the button
  int buttonState = 0;          // variable to store the read state


  delay(1000);

  // Start Bluetooth with a recognizable name
  SerialBT.begin("ESP32_PWM_Receiver");
  Serial.println("Bluetooth ready. Send signed integer (-255 to 255) to control motor.");
  Serial.println("Example: +150 (forward speed 150), -80 (reverse speed 80), 0 (stop)");
}

void loop() {

  // Read the state of the button (LOW when pressed, HIGH when released)
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
    Serial.println("Button pressed!");

    // Check for incoming Bluetooth data
    if (SerialBT.available()) {
      String cmd = SerialBT.readStringUntil('\n');  // Read until newline
      cmd.trim();                                   // Remove whitespace

      if (cmd.length() > 0) {
        int value = cmd.toInt();  // Convert to integer

        // Clamp to valid range -255 .. 255
        if (value > 255) value = 255;
        if (value < -255) value = -255;

        // Optional: deadband to avoid very low PWM jitter
        if (abs(value) < 5) value = 0;

        // Apply speed and direction
        if (value > 0) {
          // Forward
          digitalWrite(enPin, HIGH);
          analogWrite(pwmPin, value);
          Serial.printf("Forward speed: %d\n", value);
        } else if (value < 0) {
          // Reverse
          digitalWrite(enPin, LOW);
          int speed = -value;
          analogWrite(pwmPin, speed);
          Serial.printf("Reverse speed: %d\n", speed);
        } else {
          // Stop
          analogWrite(pwmPin, 0);
          Serial.println("Stop");
        }
      }
    }

  } else {

    Serial.println("Button released");

    // Reading potentiometer value
    //potValue = analogRead(potPin);
    int rawValue = analogRead(potPin);
    int percentage = map(rawValue, 0, 4095, 0, 255);
    int calnumber;

    if (percentage > 127) {
      digitalWrite(enPin, HIGH);
      // Serial.printf(">50 Percent ", percentage);
      calnumber = map(rawValue, 2047, 4095, 0, 255);
    }
    if (percentage < 126) {
      digitalWrite(enPin, LOW);
      // Serial.printf("<50 Percent " ,percentage);
      calnumber = map(rawValue, 0, 2047, 0, 255);
      calnumber = 255 - calnumber;
    }
    // Serial.println(percentage);
    //if(calnumber >=01){
    // calnumber = 0;
    //}
    if (calnumber <= 5) {
      calnumber = 0;
    }
    analogWrite(pwmPin, calnumber);
    Serial.println("test   " + String(calnumber));
    delay(500);
  }
  delay(100);  // Simple debounce delay (optional, adjust as needed)
  delay(10);   // Small delay for stability
}







 
