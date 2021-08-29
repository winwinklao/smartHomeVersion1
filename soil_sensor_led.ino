int led1 = 14;

void setup() {
// initialize serial communication at 9600 bits per second:
pinMode(led1, OUTPUT);
Serial.begin(9600);
}


void loop() {
// read the input on analog pin 0:
int sensorValue = analogRead(4); 

if (sensorValue >= 2000&& sensorValue <=4095){
Serial.println(sensorValue);
    digitalWrite(led1, HIGH);
  }
  else {
Serial.println(sensorValue);
    digitalWrite(led1, LOW);
  }
delay(1000);
}
