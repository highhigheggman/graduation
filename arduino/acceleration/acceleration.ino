const int analogPin00 = A0;
const int analogPin01 = A1;
const int analogPin02 = A2;

int xAxis = 0;
int yAxis = 0;
int zAxis = 0;

void setup() {
  //baud:9600, Even parity
  Serial.begin(9600, SERIAL_8E1);
}

void loop() {
  //read sensor val
  zAxis = analogRead(analogPin00);
  yAxis = analogRead(analogPin01);
  xAxis = analogRead(analogPin02);

  //send sensor val
  //Serial.println(xAxis, DEC);
  //Serial.println(yAxis, DEC);
  Serial.println(zAxis, DEC);

  Serial.flush();
  delay(50);
}
