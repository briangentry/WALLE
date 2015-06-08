#include <NewPing.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <math.h>



TinyGPS gps;
SoftwareSerial serialgps(6,10); 
SoftwareSerial speakjetserial(5,9);

NewPing ultra(4, 2, 400);

int Satellites = 0, gL;
float latitude = 0, longitude = 0;
float oldlat, oldlong, newlat, newlong, courseway;
boolean usingGPS = 1;

float distances[5];

float courses[5];
float reversedcourses[5];
int times[5];

char finished[] = {186, 129, 141, 129, 8, 189, 191, 255};
char calculating[] = {194, 8, 132, 159, 194, 158, 139, 145, 8, 130, 192, 128, 143, 255};
char hello[] = {20, 96, 21, 114, 22, 88, 23, 5, 183, 7, 159, 146, 164, 255};

unsigned long chars;
unsigned short sentences, failed_checksum;

/*float courses[100];
float reversedcourses[100];
int k = 0;
int times[100];*/



void setup() {
  // put your setup code here, to run once:
  // 0, 1 Unused
  // 2 Ultrasonic output
  pinMode(3, OUTPUT); // 3 left motor speed
  pinMode(4, OUTPUT); // 4 Ultrasonic return
  pinMode(5, OUTPUT);
  digitalWrite(5, HIGH);
  // 6 gps output
  pinMode(7, OUTPUT); // 7 left motor on
  pinMode(8, OUTPUT); // 8 front motor on
  // 9 speakjet serial output
  // 10 gps return
  pinMode(11, OUTPUT); // 11 left motor speed
  pinMode(12, OUTPUT); // 12 right motor direction
  pinMode(13, OUTPUT); // 13 left motor direction
  Serial.begin(115200);
  speakjetserial.begin(9600);
  serialgps.begin(4800);
  usingGPS = GPSTest();
}

void GPSAll() {
  Satellites = 0;
  while (Satellites == 0){
    while(!serialgps.available()){}
    while(serialgps.available()) {
      int c = serialgps.read(); 
      if(gps.encode(c)) {
        gps.f_get_position(&latitude, &longitude);
        Satellites = gps.satellites();
        gps.stats(&chars, &sentences, &failed_checksum);
      }
    }
  }
}

boolean GPSTest(){
  int o = 0;
  while (o < 100){
    if (serialgps.available()){
      return true;
    } else {
      o++;
    }
  }
  return false;
}

float avgLat(){
  int f = 0;
  float ret = 0;
  while (f < 5){
    delay(20);
    GPSAll();
    ret = ret + latitude;
    f ++;
  }
  return ret / f;
}

float avgLong(){
  int w;
  float ret = 0;
  while (w < 5){
    delay(20);
    GPSAll();
    ret += (longitude, 10);
    w ++;
  }
  return ret / w;
}

long ultraSonic(){
  float in, ret = 0;
  byte l = 0;
  while(l < 5){
    in = ultra.ping()/US_ROUNDTRIP_CM;
    if (in == 0) {
      in = 400;
    }
    distances[l] = in;
    l ++;
    delay(1);
  }
  ret = distComp();
  Serial.print("  ");
  Serial.println(ret);
  return ret;
}

float distComp(){
  float rett = 0;
  Serial.println();
  for (int r = 0; r < 5; r ++){
    Serial.println(distances[r]);
    boolean biggest = 1;
    boolean smallest = 1;
    for (int w = 0; w < 5; w ++){
      Serial.print(distances[r]);
      Serial.print(" : ");
      Serial.println(distances[w]);
      if (distances[r] > distances[w]){
        smallest = false;
      } else if (distances[r] < distances[w]){
        biggest = false;
      }
    }
    if (biggest || smallest){
    } else {
      rett += distances[r];
      Serial.print("    ");
      Serial.println(distances[r]);
    }
  }
  return (rett / 3);
}

void readcourse() {
  newlat = avgLat();
  newlong = avgLong();
  float deltalat = newlat - oldlat;
  float deltalong = newlong - oldlong;
  if (deltalat >= 0 && deltalong >= 0) {
    courseway = 180 / 3.1416 * atan (deltalong/deltalat) - 50;
  } else if (deltalat >= 0 && deltalong < 0) {
    courseway = 270 - 180 / 3.1416 * atan(deltalat/deltalong) - 50;
  } else if (deltalat < 0 && deltalong >= 0) {
    courseway = 90 - 180 / 3.1416 * atan(deltalat / deltalong) - 50;
  } else if (deltalat < 0 && deltalong < 0) {
    courseway = 180 + 180 / 3.1416 * atan(deltalong/deltalat) - 50;
  }
  oldlat = newlat;
  oldlong = newlong;
  courses[gL] = courseway;
  gL++;
}

void convertarray() {
  for (int i = 5; i > 0; i--) {
    if(courses[i] <= 180) {
      reversedcourses[-i + 5] = courses[i] + 180;
    } else {
      reversedcourses[-i + 5] = courses[i] - 180;
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

void forward(){
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW); // direction right
  digitalWrite(12, LOW); // direction left
  analogWrite(3, 85); // motor left
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

int drive(){
  int init = millis();
  forward();
  while(ultraSonic() > 35){
    delay(10);
  }
  init = millis() - init;
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
  return init;
}

void timeDrive(int time){
  forward();
  delay(time);
  stopp();
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
  speakjetserial.print(calculating);
  digitalWrite(12, LOW);
  rotate(33.0);
  digitalWrite(12, HIGH);
  long distance = 0;
  float pause;
  long u;
  int nAngle;
  delay(1000);
  short pauses[] = {5, 7, 8, 10, 11, 14};
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
  rotate(23.0);
  return nAngle;
}

void GPSDrive(){
  gL = 0;
  while (gL < 5){
    times[gL] = drive();
    newlat = avgLat();
    newlong = avgLong();
    readcourse();
    gL ++;
  }
  int cD = courses[4];
  gL = 0;
  convertarray();
  while (gL < 5){
    int turn = cD - reversedcourses[gL];
    if (turn > 180){
      right(turn - 180);
    } else {
      left(turn);
    }
    timeDrive(times[5 - gL]);
    cD = courses[gL];
    gL ++;
  }
  speakjetserial.print(finished);
}

void loop(){
  /*speakjetserial.print(hello);
  usingGPS = 0;
  if (usingGPS){
    oldlat = avgLat();
    oldlong = avgLong();
    GPSDrive();
  } else {
    while(true){
      drive();
    }
  }*/
  ultraSonic();
  delay(1000);
}
