// send sensor info
// http://akizukidenshi.com/download/ds/akizuki/AKI-KXR94-Module.pdf

const int analogPin00 = A0;
const int analogPin01 = A1;
const int analogPin02 = A2;

int xAxis = 0;
int yAxis = 0;
int zAxis = 0;

void setup() {
  // baud:9600, Even parity
  Serial.begin(9600, SERIAL_8E1);
}

void loop() {
  // read sensor val
  xAxis = analogRead(analogPin00);
  yAxis = analogRead(analogPin01);
  zAxis = analogRead(analogPin02);

  // Normalization
  // 3.3V: 0-675, 5.0V: 0-1024
  //xAxis = map(xAxis, 0, 675, 0, 255);
  yAxis = map(yAxis, 0, 675, 0, 255);
  zAxis = map(zAxis, 0, 675, 0, 255);

  // send sensor val
  Serial.println(xAxis, DEC);
  //Serial.println(yAxis, DEC);
  //Serial.write(zAxis);

  delay(50);
}
