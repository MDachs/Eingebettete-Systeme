#include <Servo.h>

Servo servo1;
Servo servo2;

void setup() {

  Serial.begin(115200);

  servo1.attach(7);
  servo2.attach(6);
}

void loop() {
  int xOut = analogRead(A0);
  int yOut = analogRead(A1);
  printf("xout: %d  ", xOut);
  printf("yout: %d\n", yOut);

  xOut = map(xOut, 0, 1023, 0, 180);
  yOut = map(yOut, 0, 1023, 0, 180);
  servo1.write(xOut);
  servo2.write(yOut);
  delay(20);
}
