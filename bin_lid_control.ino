/* /YIJC/2018/MA/iMad/FYP/1/Group7
   Copyright (c) 2018 Sakthiesh, Bai Cheng, Jared Recomendable, Emily Law
   Date Created: 2018-07-07, Saturday.
   Version 0.11
   Build 20180711164500

   ABOUT THIS PROGRAM
   Triggers the opening of the bin lid when a user activates the bin,
   or closes the bin lid if it is open and the user removes an object
   from the ultrasonic sensor. By default, the bin lid is closed.

   NOTES
   pinMode(pin, mode) is used to setup a specific pin number on the Arduino
   Board.
   
   -> param pin refers to the number of the pin on the Arduino Board that is
   intended to be set up.
   
   -> param mode specifies the use of the pin on the Arduino Board that is
      intended to be edited.
      
   -> -> INPUT means the pin accepts current from device (e.g. sensor) but
         cannot provide high current to device (e.g. motor); the function is
         used with digitalRead(pin), which returns HIGH or LOW.
         
   -> -> INPUT_PULLUP means the same as INPUT, but inverts the state of the
         current, acting as a 'NOT' gate (e.g. if HIGH was received, the Arduino
         will intepret the signal as LOW, and vice versa).
         
   -> -> OUTPUT means the pin provides can provide a HIGH or LOW current to
         device (e.g. motor), but is not sensitive to current from device (e.g.
         sensor); the function is used with digitalWrite(ledPin, val), which
         sends HIGH or LOW.
         
   delay(ms) is used to pause the program for ms milliseconds.
   
   delayMicroseconds(us) is used to pause the program in us microseconds.
   
   Servo.attach(pin) is used when engaging the servo motor using the Arduino
   Board, before specifying any movement of the servo motor.
   
  -> param pin refers to the number of the pin in which the servo motor is
     connected to. Currently, 9 and 10 are supported.
     
   Servo.detach() is used to unengage the servo motor from the Arduino Board,
   after specifying every movement of the servo motor beforehand.
*/

// Initialise the Libraries
#include <Servo.h>

// Initialise the Variables - Ports
Servo servo;
int servoPin = 9;
int buzzerPin = 8;
int trigPinOutsideBin = 12;
int echoPinOutsideBin = 13;
int actualDistance = 50;
int triggerDistance = 15;
bool lidIsOpen = false;

// Initialise the Process during Run
void setup() {
  servo.attach(servoPin);
  pinMode(trigPinOutsideBin, OUTPUT);
  pinMode(echoPinOutsideBin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  servo.write(0); // reset position of lid to closed position
  delay(1000);
  servo.detach();

}

// Processes during Run
long measureDistance(int senderPin, int receiverPin) {
  /* Measure the distance between the sensor and the object.
     The senderPin is usually the trigPin and the receiverPin is usually the
     echoPin, following conventions.
  */
  long duration;
  long distance;

  digitalWrite(senderPin, LOW);
  delayMicroseconds(5);
  digitalWrite(senderPin, HIGH);
  delayMicroseconds(15);
  digitalWrite(senderPin, LOW);

  duration = pulseIn(receiverPin, HIGH);
  distance = duration / 52.5;

  return distance;

}

int measureAverageDistance(int senderPin, int receiverPin) {
  /* Measure the average distance between the sensor and the object by running
     measureDistance() three times.
  */
  int averageDistance;
  int count = 2;
  int totalDistance;

  while (count >= 0) {
    totalDistance += measureDistance(senderPin, receiverPin);
    count--;
    delay(16);
  }

  averageDistance = totalDistance / 3;

  return averageDistance;

}

void openBinLid() {
  /* Open the bin's lid.
  */
  servo.attach(servoPin);
  delay(8);
  servo.write(80);
  delay(800);
  servo.detach();

}

void closeBinLid() {
  /* Close the bin's lid.
  */
  servo.attach(servoPin);
  delay(8);
  servo.write(0);
  delay(800);
  servo.detach();

}

void loop() {
  actualDistance = measureAverageDistance(trigPinOutsideBin, echoPinOutsideBin);

  if (!lidIsOpen && actualDistance < triggerDistance) {
    openBinLid();
    tone(buzzerPin, 1000);
    delay(300);
    noTone(buzzerPin);
    lidIsOpen = true;

  } else if (lidIsOpen && actualDistance > triggerDistance) {
    delay(1200);
    closeBinLid();
    lidIsOpen = false;

  }

}
