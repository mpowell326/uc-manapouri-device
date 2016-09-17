const int ledPin =  8;      // the number of the LED pin
const int battery1mosfet = A0;
const int battery1 = A1;
const int battery2 = A2;
const int battery2mosfet = A3;
const int sensorsoff = A4;
const int button = 9;

int ledState = LOW;             // ledState used to set the LED
int z = 10;
int i;
int sensorValue;
int buttonval;

unsigned long previousMillis = 0;        // will store last time LED was updated

const long interval = 500;           // interval at which to blink (milliseconds)


// the setup routine runs once when you press reset:
void setup() {

  pinMode(ledPin, OUTPUT);
  pinMode(battery1mosfet, OUTPUT);
  pinMode(battery2mosfet, OUTPUT);
  pinMode(sensorsoff, OUTPUT);
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);  // initialize serial communication at 9600 bits per second:
  digitalWrite(battery1mosfet, HIGH);   
  digitalWrite(battery2mosfet, HIGH);   
}

// the loop routine runs over and over again forever:
void loop() {

  Mosfetvoltage1();
  Mosfetvoltage2();
  button();
  
}

int Mosfetvoltage1(){
  sensorValue = 0;
  for (i=0; i<z; i++){
    
  sensorValue = sensorValue + analogRead(battery1); // read the input on analog pin 1:
  
  }
  sensorValue = sensorValue/z; //creates average depending on amount of values taken
 
  float voltage = sensorValue * (5.0 / 1023.0);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
   
  Serial.println(voltage); // print out the value you read:
  
  if (voltage < X){
    digitalWrite(battery1mosfet, LOW);   
    //gpio here to turn of the mosfet
  }
}

int Mosfetvoltage2(){
  sensorValue = 0;
  for (i=0; i<z; i++){
    
  sensorValue = sensorValue + analogRead(battery2); // read the input on analog pin 2:
  
  }
  sensorValue = sensorValue/z; //creates average depending on amount of values taken
 
  float voltage = sensorValue * (5.0 / 1023.0);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
   
  Serial.println(voltage); // print out the value you read:
  
  if (voltage < X){
    digitalWrite(battery2mosfet, LOW);   
    //gpio here to turn of the mosfet
  }
}


int LEDLIGHT(){
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(ledPin, ledState);
  }
}

int button(){
  buttonval = digitalRead(button);
  if (buttonval = LOW){
    LEDLIGHT(); //lets the user know that the shutdown sequence has started
    delay(5000); // delay of 5seconds for the equipment to turn off
    digitalWrite(battery1mosfet, LOW);   
    digitalWrite(battery2mosfet, LOW);
  }
}



