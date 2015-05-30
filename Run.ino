#include <SoftwareSerial.h> 
#include <TinyGPS.h>
#include <math.h>

int angle1; 
int angle;
long distance = 0; 
long distancefromwall = 400; 

int i = 0; //used for storing angles to array
float courses[100];
float reversecourses[100];

//1, 2, 9, 10
//7 & 8 determine if movement mortor used, or ultrasonic motor
//byte trigPin = 4; 
byte echoPin = 5; //ultrasonic sensor pins
byte rotatingPin1 = 6; //ultrasonic rotation
byte dir1Pin = 13; //direction of left motor; opposite of dir2Pin if forward
byte motor1Pin = 11; //left
byte dir2Pin = 12; //direction of right motor
byte motor2Pin = 3;//right
byte rxspeakjet = ###;
byte txspeakjet = ###;
byte rxgps = ###;
byte txgps = ###;

TinyGPS gps;
SoftwareSerial serialgps(txgps, rxgps); //not actual pins; used for transmission of data
SoftwareSerial speakjetserial(rxspeakjet, txspeakjet);

char finished[] = {186, 129, 141, 129, 8, 189, 191, 255};
char calculating[] = {194, 8, 132, 159, 194, 158, 139, 145, 8, 130, 192, 128, 143, 255};

int year, Satellites; 
float latitude, longitude;
unsigned long chars;
unsigned short sentences, failed_checksum; //variables for GPS

void GPSlatlong() {
  while(!serialgps.available()){
  }
  while(serialgps.available()) {
    int c = serialgps.read();
    if(gps.encode(c)) {
      gps.f_get_position(&latitude, &longitude);
    }
  }
} //detects lat and long

float oldlat;
float oldlong;
float newlat;
float newlong;
float courseway; //variables for detecting direction

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
  courses[i] = courseway;
  i++;
} //detects the direction using right triangles!



long ultraSonic() { // code comes from Rui Santos;
//http://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
  digitalWrite(trigPin, LOW);
  delay(5);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW); // send sound out
  long duration = pulseIn(echoPin, HIGH);
  long cm = (duration/2) / 29.1; 
  return cm;
} //detects distance from wall

void rotate(int time) {
  analogWrite(rotatingPin1, 255);
  delay(time);
  analogWrite(rotatingPin1, 0);
  delay(1000);
} //rotates the ultrasonic sensor


void turn() {
  if (angle1 < 90) {
    analogWrite(dir1Pin, 250);
    analogWrite(motor1Pin, 255); // turn motor1 forward
    analogWrite(dir2Pin, 250);
    analogWrite(motor2Pin, 255); //turn motor2 backward
    delay(angle1);
  } else if(angle == 90) {
  } else if(angle1 > 90) {
    analogWrite(dir1Pin, 100);
    analogWrite(motor1Pin, 255); // turn motor1 backward
    analogWrite(dir2Pin, 100);
    analogWrite(motor2Pin, 255); //turn motor2 forward
    delay(angle1);
  }
} //turns the vehicle

void forward() {
  analogWrite(dir1Pin, 250);
  analogWrite(dir2Pin, 100);
  analogWrite(motor1Pin, 255);
  analogWrite(motor2Pin, 255);
  delay(100);
  distancefromwall = ultraSonic();
} //goes straight forward

void detectdistance() {
  // put your main code here, to run repeatedly:
  while (distancefromwall >= 10) { //as long as it's at least 10 cm away keep going
    forward();
  }
  if (distancefromwall < 10) {
    angle = 0;
    long distance = 0; 
    analogWrite(13, 100);
    rotate(164);
    analogWrite(13, 250);
    for(angle = 0; angle <= 180; angle+=30) {
      long oldDistance = distance;
      rotate(9.8);
      distance = ultraSonic();
      if (distance > oldDistance) {
        angle1 = angle;
      } else {
      }
    }
    analogWrite(13, 100);
    rotate(164);
    distancefromwall = ultraSonic();
  }
  distancefromwall = ultraSonic();
}

void convert() {
  for (int j = 0; j < 100; j++) {
      if (courses[j] <= 180) {
        reversecourses[j] = courses[j] + 180;
      } else {
        reversecourses[j] = 360 - courses[j];
      }
    }
} //converts it into the reverse of what it has done

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  serialgps.begin(4800);
  speakjetserial.begin(9600);
  pinMode(rxspeakjet, INPUT);
  pinMode(txspeakjet, OUTPUT);
  pinMode(trigPin, OUTPUT); //ultrasonic stuff
  pinMode(echoPin, INPUT); //ultrasonic stuff
  Serial.begin(9600);
  pinMode(motor1Pin, OUTPUT);
  pinMode(motor2Pin, OUTPUT);
  pinMode(dir1Pin, OUTPUT);
  pinMode(dir2Pin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (color != blue) {
    detectdistance();
    readcourse();
  }
  if (color = blue) {
    speakjetserial.print(finished);
    convert();
    angle1 = 180;
    turn();
    float deltadegree;
    for (int j = 0; j < 100; j++) {
      while (distancefromwall >= 10) {
        forward();
      }
      if (distancefromwall < 10) {
        speakjetserial.print(calculating);
        deltadegree = reversecourses[j+1] - reversecourses[j];
        angle1 = deltadegree;
        turn();
      }
      
    }
  }
}
