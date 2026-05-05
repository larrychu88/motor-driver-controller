// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6)
//#include <BluetoothSerial.h>
#include <esp_now.h>
#include <WiFi.h>

const int buttonPin = 4;           // GPIO pin connected to the button (other side to GND)
const int drive_selected = 32;     //select drive data
const int rotation_selected = 33;  //select rotation data
const int cutter_selected = 26;    //select cutterhead data
const int camera_selected = 27;    //select camera_turn data

int buttonState = 0;  // Variable to store the read state

const int potPin = 34;
const int pwmPin = 16;
const int enPin = 18;

// Bluetooth object
//BluetoothSerial SerialBT;

typedef struct struct_message {
  char a[32];
  int b1;
  int b2;
  int b3;
  int b4;
  int c1;
  int c2;
  int c3;
  int c4;
  bool d;
} struct_message;

struct_message myData;

//callback function that will be executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Ignore mac completely – just print the message
//  Serial.print("Bytes REceived");
//  Serial.println(len);
//  Serial.print("Char: ");
//  Serial.println(myData.a);
 // Serial.println("Int: ");
 // Serial.println(myData.b1);
 // Serial.println(myData.b2);
 // Serial.println(myData.b3);
 // Serial.println(myData.b4);
 // Serial.println("Float: ");
 // Serial.println(myData.c1);
 // Serial.println(myData.c2);
 // Serial.println(myData.c3);
 // Serial.println(myData.c4);
 // Serial.print("Bool: ");
 // Serial.println(myData.d);
 // Serial.println();

  //Serial.println(myData.msg);
}


// variable for storing the potentiometer value
int potValue = 0;
int value = 0;

void setup() {
  //Initialize Seril Mionitor
  Serial.begin(115200);

  // Set Device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Errtor Initializing ESP-NOW");
    return;
  }

  //Once ESP-NOW is sucessfully Init, we will register for recv CB to
  // get recv packer info

  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));

  pinMode(pwmPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  // Configure pin as input with internal pull-up enabled
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(drive_selected, INPUT_PULLUP);
  pinMode(rotation_selected, INPUT_PULLUP);
  pinMode(cutter_selected, INPUT_PULLUP);
  pinMode(camera_selected, INPUT_PULLUP);

  Serial.println("ESP-NOW ready, waiting for data...");

  const int bt_on_button = 32;  // GPIO pin connected to the button
  int buttonState = digitalRead(buttonPin);          // variable to store the read state




  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("ESP-NOW init failed");
    return;
  }

  //esp_now_register_recv_cb(OnDataRecv);





  delay(1000);

  // Start Bluetooth with a recognizable name
  // SerialBT.begin("ESP32_PWM_Receiver");
  // Serial.println("Bluetooth ready. Send signed integer (-255 to 255) to control motor.");
  // Serial.println("Example: +150 (forward speed 150), -80 (reverse speed 80), 0 (stop)");
}

void loop() {

  // Read the state of the button (LOW when pressed, HIGH when released)
  buttonState = digitalRead(buttonPin);

  if (buttonState == LOW) {
  //  Serial.println("Button pressed!"); // IN BLUETOOTH MODE

    // Check for incoming Bluetooth data
    //if (SerialBT.available()) {
    //  String cmd = SerialBT.readStringUntil('\n');  // Read until newline
    //  cmd.trim();                                   // Remove whitespace
   // String cmd = "";


   // if (cmd.length() > 0) {
  //    int value = cmd.toInt();  // Convert to integer
      if (digitalRead(drive_selected) == LOW){
        value = myData.b1;
        digitalWrite(enPin, myData.c1);
      }
      if (digitalRead(rotation_selected) == LOW){
         value = myData.b2;
        digitalWrite(enPin,myData.c2);
      }
      if (digitalRead(cutter_selected) == LOW){
         value = myData.b3;
        digitalWrite(enPin,myData.c3);
      }
      if  (digitalRead(camera_selected) == LOW){
        value = myData.b4;
        digitalWrite(enPin,myData.c4);
      }
 

      // Clamp to valid range -255 .. 255
      if (value > 255) value = 255;
      if (value < -255) value = -255;

      // Optional: deadband to avoid very low PWM jitter
      if (abs(value) < 5) value = 0;

analogWrite(pwmPin, value);

//      // Apply speed and direction
//      if (value > 0) {
//        // Forward
//        digitalWrite(enPin, HIGH);
//       analogWrite(pwmPin, value);
    //    Serial.printf("Forward speed: %d\n", value);
//      } else if (value < 0) {
        // Reverse
//        digitalWrite(enPin, LOW);
//        int speed = -value;
//        analogWrite(pwmPin, speed);
//       Serial.printf("Reverse speed: %d\n", speed);
//      } else {
        // Stop
 //       analogWrite(pwmPin, 0);
     //   Serial.println("Stop");
 //     }
//    }


  } else {

   // Serial.println("Button released");

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
    //Serial.println("test   " + String(calnumber));
  //  delay(500);
  }
 // delay(100);  // Simple debounce delay (optional, adjust as needed)
  delay(10);   // Small delay for stability
}





 
