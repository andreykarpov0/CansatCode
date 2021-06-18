

void setup() {
  // put your setup code here, to run once:
  pinMode(PG3, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(PG3, HIGH);
  delay(500);
  digitalWrite(PG3, LOW);
  delay(500);
}
