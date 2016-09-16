
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  digitalWrite(pin, HIGH);   
  digitalWrite(pin, HIGH);   
}

// the loop routine runs over and over again forever:
void loop() {

  Mosfetvoltage1();
  
  
}

int Mosfetvoltage1(){
  int z = 10;
  int i;
  int sensorValue = 0;
  for (i=0; i<z; i++){
    
  sensorValue = sensorValue + analogRead(A0); // read the input on analog pin 0:
  
  }
  sensorValue = sensorValue/z; //creates average depending on amount of values taken
 
  float voltage = sensorValue * (5.0 / 1023.0);  // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
   
  Serial.println(voltage); // print out the value you read:
  
  if (voltage < X){
    digitalWrite(pin, LOW);   
    //gpio here to turn of the mosfet
  }


  return 
}





