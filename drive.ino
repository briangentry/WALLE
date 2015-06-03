#include <Ultrasonic.h>

Ultrasonic ultra(4, 2);

float ultraSonic(){
  return ultra.Timing() / 29.1 / 2;
}

void forward(){
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW); // direction right
  digitalWrite(12, LOW); // direction left
  analogWrite(3, 85); // motor left
  analogWrite(11, 255); // motor rigth
}

void backward(){
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, HIGH); // direction right
  digitalWrite(12, HIGH); // direction left
  analogWrite(3, 85); // motor left
  analogWrite(11, 255); // motor rigth
}

void right(int angle){
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, HIGH); // direction right
  digitalWrite(12, LOW); // direction left
  analogWrite(3, 130); // motor left
  analogWrite(11, 255); // motor rigth
  delay(angle*7.5);
  stopp();
}

void left(int angle){
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW); // direction right
  digitalWrite(12, HIGH); // direction left
  analogWrite(3, 130); // motor left
  analogWrite(11, 255); // motor rigth
  delay(angle*10.4);
  stopp();
}

void stopp(){
  digitalWrite(7, LOW);
  analogWrite(11, 0);
  analogWrite(3, 0);
}

void setup() {
  // put your setup code here, to run once:
  pinMode(8, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  Serial.begin(115200);
}

void drive(){
  forward();
  while(ultraSonic() > 25){}
  stopp();
  delay(100);
  backward();
  delay(250);
  stopp();
  int dir = newPath();
  if (dir > 0){
    left(dir);
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
  int distance = 0;
  float pause, u;
  int nAngle;
  delay(1000);
  short pauses[] = {5, 6, 8, 10, 14, 24};
  for (int i = -90; i < 90; i += 30){
    u = ultraSonic();
    if (distance < u){
      nAngle = i;
      distance = u;
    }
    rotate(pauses[(i+90)/30]);
    delay(90);
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
