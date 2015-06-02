void forward(){
  digitalWrite(7, HIGH);
  digitalWrite(8, LOW);
  digitalWrite(13, LOW); // direction right
  digitalWrite(12, LOW); // direction left
  analogWrite(3, 130); // motor left
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
}

void loop() {
  // put your main code here, to run repeatedly:
  left(30);
  delay(1000);
  left(60);
  delay(1000);
  left(90);
  delay(1000);
  left(180);
  delay(1000);
}
