#define acidpin A0
#define wetpin A1
#define MBEE_SERIAL Serial1

void setup() {
  // put your setup code here, to run once:
  MBEE_SERIAL.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  byte wetval = analogRead(wetpin);
  byte acidval = analogRead(acidpin);
  MBEE_SERIAL.write(highByte(wetval));
  MBEE_SERIAL.write(lowByte(wetval));
  MBEE_SERIAL.write(highByte(acidwal));
  MBEE_SERIAL.write(lowByte(acidwal));
  delay(100);
}
