#include <SoftwareSerial.h> 
#include <TinyGPS.h>
//#include <math.h>

TinyGPS gps;
SoftwareSerial serialgps(6,10); 
SoftwareSerial speakjetserial(9,5);

byte trigPin = 4; 
//byte echoPin = 5; //ultrasonic sensor pins
byte dir1Pin = 13; //direction of left motor; opposite of dir2Pin if forward
byte motor1Pin = 11; //left
byte dir2Pin = 12; //direction of right motor
byte motor2Pin = 3;//right
byte rxspeakjet = 9;
byte txspeakjet = 5;

unsigned char finished[] = {186, 129, 141, 129, 8, 189, 191, 255};
char calculating[] = {194, 8, 132, 159, 194, 158, 139, 145, 8, 130, 192, 128, 143, 255};

int globaL = 0;
int Satellites = 0;
float latitude, longitude;
float courseLats[100];
float courseLong[100];
float reversecourses[100];
float oldlat, oldlong, newlat, newlong, courseway;

unsigned long chars;
unsigned short sentences, failed_checksum;

void GPSAll(){
  Satellites = 0;
  while(Satellites < 1 || Satellites == 255){
    while(serialgps.available()){
      int c = serialgps.read(); 
      if(gps.encode(c)){
        gps.f_get_position(&latitude, &longitude);
        Satellites = gps.satellites();
        gps.stats(&chars, &sentences, &failed_checksum);
      }
    }
  }
}
/*
void readcourse() {
  int i;
  GPSlatlong();
  newlat = latitude;
  newlong = longitude;
  float deltalat = newlat - oldlat;
  float deltalong = newlong - oldlong;
  if (deltalat >= 0 && deltalong >= 0) {
    courseway = 180 / 3.1416 * atan (deltalong/deltalat);
  } else if (deltalat >= 0 && deltalong < 0) {
    courseway = 270 - 180 / 3.1416 * atan(deltalat/deltalong);
  } else if (deltalat < 0 && deltalong >= 0) {
    courseway = 90 - 180 / 3.1416 * atan(deltalat / deltalong);
  } else if (deltalat < 0 && deltalong < 0) {
    courseway = 180 + 180 / 3.1416 * atan(deltalat/deltalong);
  }
  oldlat = newlat;
  oldlong = newlong;
  courses[i] = courseway;
  i++;
}

// for different sensor
long ultraSonic(){
  digitalWrite(trigPin, LOW);
  delay(5);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW); // send sound out
  long duration = pulseIn(echoPin, HIGH);
  long cm = (duration/2) / 29.1; 
  return cm;
}*/


long ultraSonic(){
  pinMode(4, OUTPUT);
  digitalWrite(4, LOW);
  delayMicroseconds(2);
  digitalWrite(4, HIGH);
  delayMicroseconds(5);
  digitalWrite(4, LOW);
  pinMode(4, INPUT);
  float ret = pulseIn(4, HIGH);
  return (ret / 2) / 29.1;
}

void rotate(float time){
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  analogWrite(3, 255);
  delay(time);
  analogWrite(3, 0);
  digitalWrite(8, LOW);
  delay(1000);
}

float newPath(){
  digitalWrite(12, LOW);
  rotate(37.0);
  //speakjetserial(finished); not working
  digitalWrite(12, HIGH);
  float distance = 0, pause = 7.0, u;
  int nAngle;
  for (int i = -90; i < 90; i += 30){
    u = ultraSonic();
    if (distance < u){
      nAngle = i;
      distance = u;
    }
    pause = 8.5 + (i / 60.0);
    if (pause == 9){
      pause = 9.5;
    } else if (pause == 9.5){
      pause = 11.5;
    } else if (pause == 10){
      pause = 13;
    }
    rotate(pause);
  }
  digitalWrite(12, LOW);
  rotate(22.0);
  return nAngle;
}

/*void convert() {
  for (int j = 0; j < 100; j++) {
    if (courses[j] <= 180) {
      reversecourses[j] = courses[j] + 180;
    } else {
      reversecourses[j] = 360 - courses[j];
    }
  }
}
*/
void l(boolean f){
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
  byte sl;
  if (f){
    digitalWrite(12, LOW);
    sl = 70;
  } else {
    digitalWrite(12, HIGH);
    sl = 130;
  }
  for(short x = 0; x < sl; x++){
    analogWrite(3, x);
  }
}

void r(boolean f){
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  byte s;
  if (f){
    digitalWrite(13, LOW);
    s = 255;
  } else {
    digitalWrite(13, HIGH);
    s = 100;
  }
  for(byte x = 0; x < s; x++){
    analogWrite(11, x);
  }
}

void s(){
  digitalWrite(7, LOW);
  analogWrite(11, 0);
  analogWrite(3, 0);
}

void turn(int angle){
  int k = angle;
  if (k > 0){
    r(true);
    l(false);
  } else {
    r(false);
    l(true);
    k = 0 - k;
  }
  delay(k*12);
  s();
}

void drive(){
  int dist = ultraSonic();
  r(true);
  l(true);
  while(dist > 30){
    dist = ultraSonic();
  }
  s();
  turn(newPath());
  delay(1000);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT); //ultrasonic stuff
  //pinMode(echoPin, INPUT); //ultrasonic stuff
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  serialgps.begin(4800);
  speakjetserial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly;
  drive();
  GPSAll();
}
