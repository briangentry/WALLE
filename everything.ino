#include <SoftwareSerial.h>
#include <TinyGPS.h>

TinyGPS gps;
SoftwareSerial serialgps(6,10);
SoftwareSerial speakjetserial(5,9);

int Satellites, globaL = 0;
float distance = 0, distanceToWall = 1000;
float latitude, longitude;
float courseLats[100];
float courseLong[100];
float reversecourses[100];
float oldlat, oldlong, newlat, newlong, courseway;

unsigned long chars;
unsigned short sentences, failed_checksum;

char finished[] = {186, 129, 141, 129, 8, 189, 191, 255};
char calculating[] = {194, 8, 132, 159, 194, 158, 139, 145, 8, 130, 192, 128, 143, 255};

void setup(){
  pinMode(4, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  Serial.begin(115200);
  speakjetserial.begin(9600);
  serialgps.begin(4800);
}

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
  speakjetserial.print(calculating);
  digitalWrite(12, LOW);
  rotate(20.0);
  digitalWrite(12, HIGH);
  distance = 0;
  float pause = 7.0, u;
  int nAngle;
  for (int i = -90; i <= 90; i += 30){
    u = ultraSonic();
    if (distance < u){
      nAngle = i;
      distance = u;
    }
    pause = 7.5 + (i / 25.0);
    if (i > 0){
      pause += 2;
    }
    rotate(pause);
    delay(90);
  }
  digitalWrite(12, LOW);
  rotate(20.0);
  return nAngle;
}

void l(boolean f){
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
  digitalWrite(8, LOW);
  digitalWrite(7, HIGH);
  byte sl;
  if (f){
    digitalWrite(12, LOW);
    sl = 110;
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
  byte sr;
  if (f){
    digitalWrite(13, LOW);
    sr = 255;
  } else {
    digitalWrite(13, HIGH);
    sr = 120;
  }
  for(byte x = 0; x < sr; x++){
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
  delay(k*20);
  s();
}

void drive(){
  distanceToWall = 1000;
  r(true);
  l(true);
  while(distanceToWall > 20 || distanceToWall == 0 || distanceToWall == 6){
    distanceToWall = ultraSonic();
    delay(300);
  }
  s();
  int chosenAngle = newPath();
  if (distance < 20){
    turn(180);
    chosenAngle = newPath();
  } else {
    turn(chosenAngle);
  }
  delay(50);
}

void GPSAll(){
  Satellites = 0;
  while(Satellites < 1 || Satellites == 255){
    while(!serialgps.available()){}
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

void loop(){
  GPSAll();
  courseLats[globaL] = latitude;
  courseLong[globaL] = longitude;
  globaL ++;
  drive();
  delay(2000);
}
