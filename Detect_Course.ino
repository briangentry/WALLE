#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <math.h>

TinyGPS gps;
SoftwareSerial serialgps(2,3);

int year, Satellites;
float latitude, longitude;
unsigned long chars;
unsigned short sentences, failed_checksum;

void GPSlatlong() {
  while(!serialgps.available()){
  }
  while(serialgps.available()) {
    int c = serialgps.read();
    if(gps.encode(c)) {
      gps.f_get_position(&latitude, &longitude);
    }
  }
}

float oldlat;
float oldlong;
float newlat;
float newlong;
float courseway;

void readcourse() {
  while(Satellites < 1) {
    GPSlatlong();
  }
  GPSlatlong();
  newlat = latitude;
  newlong = longitude;
  Satellites = 0;
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
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  serialgps.begin(4800);
}

void loop() {
  // put your main code here, to run repeatedly:
  readcourse();
  delay(5000);
}
