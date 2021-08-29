#define Trig 2
#define Echo 3

const int speek = 4 ;

#include <Servo.h>
Servo myservo ;
long duration ;
int distance;

void setup() {
  myservo.attach(4) ;
  pinMode(speek, OUTPUT);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
}

void loop() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(5);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  duration = pulseIn(Echo,HIGH) ;
  distance = duration *0.034 / 2 ;

  if (distance <= 2)
  {
    digitalWrite(speek, HIGH);
    delay(500);
    myservo.write(120) ;
    delay(5000);
  }
  else
  {
    myservo.write(0);
  }
}
