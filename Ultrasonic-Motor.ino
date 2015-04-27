int wheelPin = 3;
long cm, duration;
int trigPin = 11;
int echoPin = 10;

void setup() {
  // put your setup code here, to run once:
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT); // from Rui Santos; 
  //http://randomnerdtutorials.com/complete-guide-for-ultrasonic-sensor-hc-sr04/
  
}

void loop() {
  digitalWrite(trigPin, LOW);
  delay(5);
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW); // send sound out
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1; 
  // put your main code here, to run repeatedly:
  if (cm < 25) {
    analogWrite(wheelPin, 255);
  } else {
  analogWrite(wheelPin, cm * 10);
  }
}
