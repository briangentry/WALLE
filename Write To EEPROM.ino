#include <EEPROM.h>
#include <SoftwareSerial.h>

SoftwareSerial mySerialPort(2, 3);

char sayingstuff[20];
char message[20] = {183, 7, 159, 146, 164, 140, 155, 141, 154, 140, 8, 129, 167, 152, 177, 147, 8, 128, 142, 164};
void setup() {
  // put your setup code here, to run once:
  char message[20] = {183, 7, 159, 146, 164, 140, 155, 141, 154, 140, 8, 129, 167, 152, 177, 147, 8, 128, 142, 164};
  int i;
  for(i = 0; i < 20; i++) {
    EEPROM.write(i, message[i]);
  }
  mySerialPort.begin(9600);
  pinMode(3, OUTPUT);
  digitalWrite(3, LOW);
  for(i = 0; i < 20; i++) {
    sayingstuff[i] = EEPROM.read(i);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  
  mySerialPort.print(message);

}
