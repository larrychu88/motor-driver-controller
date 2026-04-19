// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int potPin = 34;
const int pwmPin = 16;
const int enPin = 5;

// variable for storing the potentiometer value
int potValue = 0;

void setup() {
  Serial.begin(115200);
  pinMode(pwmPin, OUTPUT);
  pinMode(enPin, OUTPUT);
  delay(1000);
}

void loop() {
  // Reading potentiometer value
  //potValue = analogRead(potPin);
  int rawValue = analogRead(potPin);
  int percentage = map(rawValue, 0, 4095, 0, 100);
  int calnumber;

  if (percentage >50){
    digitalWrite(enPin, HIGH);
   // Serial.printf(">50 Percent ", percentage);
   calnumber = map(rawValue, 2047, 4095, 0, 100);
  }
if (percentage <50){
    digitalWrite(enPin, LOW);
   // Serial.printf("<50 Percent " ,percentage);
   calnumber = map(rawValue, 0, 2047, 0, 100);
   calnumber = 100-calnumber;
  }
 // Serial.println(percentage);
 if(calnumber <=10){
  calnumber = 0;
 }
 if(calnumber <=10){
  calnumber= 0 ;
 }
  analogWrite(pwmPin, calnumber);
  Serial.println("test   "+ String(calnumber));
  delay(500);
}






 