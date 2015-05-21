char hello[] = {183, 70, 159, 137, 255};
char surprise[] = {194, 129, 8, 159, 2, 195, 153, 191, 141, 8, 128, 255};


void setup() {
  // put your setup code here, to run once:
  pinMode(#### with ~, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 0; i < 5; i++) {
    int number = hello[i];
    analogWrite(#### with ~, number);
  }
  delay(1000);
  for (int j = 0; j < 12; j++) {
    int number = surprise[j];
    analogWrite(#### with ~, number);
  }
  delay(1000);
}
