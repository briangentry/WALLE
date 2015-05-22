#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 5);


char message[] = {183, 70, 159, 137, 8, 223, 8, 221, 8, 220, 255};



void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  pinMode(5, INPUT);
  pinMode(2, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  mySerial.println(message);
  /*for (int i = 0; i < 5; i++) {
    int number = message[i];
    mySerial.println(number);
  }*/
  delay(5000);
}
