#include <Ultrasonic.h>

Ultrasonic ultra(4, 2);

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
    delay(2000);
    Serial.print(u);
    Serial.print("cm, ");
    Serial.print(i);
    Serial.println(" degrees ");
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
