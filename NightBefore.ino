/*
  either needs to be fixed
  or copied and pasted part 
  by part into a working GPS 
  file...
*/

#include <Ultrasonic.h>
#include <SoftwareSerial.h> 
#include <TinyGPS.h>
#include <math.h>

Ultrasonic ultra(4, 2);

TinyGPS gps;
SoftwareSerial serialgps(6,10); 
SoftwareSerial speakjetserial(5,9);

char finished[] = {186, 129, 141, 129, 8, 189, 191, 255};
char calculating[] = {194, 8, 132, 159, 194, 158, 139, 145, 8, 130, 192, 128, 143, 255};
char hello[] = {20, 96, 21, 114, 22, 88, 23, 5, 183, 7, 159, 146, 164, 255};

int globaL = 0;
int Satellites = 0;
float latitude, longitude;
float courseLats[100];
float courseLong[100];
float reversecourses[100];
float oldlat, oldlong, newlat, newlong, courseway;

unsigned long chars;
unsigned short sentences, failed_checksum;

void setup() {
  // put your setup code here, to run once:
  // 0, 1 Unused
  // 2 Ultrasonic output
  pinMode(3, OUTPUT); // 3 left motor speed
  pinMode(4, OUTPUT); // 4 Ultrasonic return
  // 5 designated speakjet input
  // 6 gps output
  pinMode(7, OUTPUT); // 7 left motor on
  pinMode(8, OUTPUT); // 8 front motor on
  // 9 speakjet serial output
  // 10 gps return
  pinMode(11, OUTPUT); // 11 left motor speed
  pinMode(12, OUTPUT); // 12 right motor direction
  pinMode(13, OUTPUT); // 13 left motor direction
  Serial.begin(115200);
  serialgps.begin(4800);
  speakjetserial.begin(9600);
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

void ReTrace(){
  // needs to store the last set of coordinates
  // or access them in some way
  // 
  // then take the current lat / long
  // use the difference to figure out the current orientation
  // 
  // turn a certain amount to drive toward the next lat / long pair
  // 
  // drive a certain amount of time????
  // or just drive and check GPS until close - then what happens if it misses? 
  // drive a certain amount of time.
}

long ultraSonic(){
  byte x = 0;
  float in, ret, l = 0;
  while(x < 6){
    in = ultra.timing()/58.2;
    if(in < 500){
      ret += in;
      l ++;
    }
    x ++;
    delay(1);
  }
  if (l == 0){
    l = 1;
  }
  return (ret / l);
}

void forward(){
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW); // direction right
  digitalWrite(12, LOW); // direction left
  analogWrite(3, 70); // motor left
  analogWrite(11, 255); // motor right
}

void backward(){
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, HIGH); // direction right
  digitalWrite(12, HIGH); // direction left
  analogWrite(3, 85); // motor left
  analogWrite(11, 255); // motor right
}

void right(int angle){
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, HIGH); // direction right
  digitalWrite(12, LOW); // direction left
  analogWrite(3, 130); // motor left
  analogWrite(11, 255); // motor right
  delay(angle*7.5);
  stopp();
}

void left(int angle){
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW); // direction right
  digitalWrite(12, HIGH); // direction left
  analogWrite(3, 130); // motor left
  analogWrite(11, 255); // motor right
  delay(angle*10.4);
  stopp();
}

void stopp(){
  digitalWrite(7, LOW);
  analogWrite(11, 0);
  analogWrite(3, 0);
}

void drive(){
  forward();
  while(ultraSonic() > 20){}
  stopp();
  delay(100);
  backward();
  delay(250);
  stopp();
  int dir = newPath();
  if (dir > 0){
    left(dir);
  } else if (dir == 0) {
  } else {
    right(-1 * dir);
  }
}

void rotate(float time){
  digitalWrite(7, LOW);
  digitalWrite(8, HIGH);
  analogWrite(3, 255);
  delay(time);
  analogWrite(3, 0);
  digitalWrite(8, LOW);
  delay(500);
}

float newPath(){
  digitalWrite(12, LOW);
  rotate(48.0);
  digitalWrite(12, HIGH);
  long distance = 0;
  float pause;
  long u;
  int nAngle;
  delay(1000);
  short pauses[] = {5, 6, 8, 14, 18, 24};
  for (int i = -90; i < 90; i += 30){
    u = ultraSonic();
    if (distance < u){
      nAngle = i;
      distance = u;
    }
    rotate(pauses[(i+90)/30]);
    delay(1000);
  }
  if (distance < ultraSonic()){
    nAngle = 90;
    distance = ultraSonic();
  }
  digitalWrite(12, LOW);
  rotate(25.0);
  return nAngle;
}

void loop() {
  // put your main code here, to run repeatedly:
  drive();
}
