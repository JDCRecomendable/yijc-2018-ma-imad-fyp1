/* /YIJC/2018/MA/iMad/FYP/1/Group7
   Copyright (c) 2018 Sakthiesh, Bai Cheng, Jared Recomendable, Emily Law
   Date Created: 2018-07-07, Saturday.
   Version 0.1
   Build 20180723174500_ANDEE

   ABOUT THIS PROGRAM
   Sends an SMS message to the targetNumber when the bin reaches a threshold.

   TEMP NOTES
   1. Download Andee Android to an Android device.
   2. Upgrade the college's employed Andee board's firmware to the latest
      version.
      (https://annikken.gitbooks.io/annikken-andee/content/GettingStarted/AndeeSetup.html#install-andee-firmware)
   3. Update the local Andee libraries on the development machine.
   4. Connect the Arduino/Andee Boards set to the Android device.
   5. Debug and test.
   6. Delete this TEMP NOTE when steps 1-5 above are completed.
*/

// Initialise the Libraries
#include <SPI.h>
#include <Andee.h>

// Administrative Activities
int trigPinInsideBin = 9;
int echoPinInsideBin = 10;
int actualDistance = 30;
int triggerDistance = 5;
bool messageSent = false;
int targetNumber = 0;
char message[] = "Trash Bin XXX is full. Please clear Trash Bin XXX.";

// Initialise the Process during Run
void setup() {
  Andee.begin();
  pinMode(trigPinInsideBin, OUTPUT);
  pinMode(echoPinInsideBin, INPUT);

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

// Main Run
void loop() {
  actualDistance = measureAverageDistance(trigPinInsideBin, echoPinInsideBin);

  if (!messageSent && actualDistance < triggerDistance) {
    Andee.sendSMS(targetNumber, message);
    messageSent = true;

  } else if (messageSent && actualDistance > triggerDistance) {
    messageSent = false;

  }

}
