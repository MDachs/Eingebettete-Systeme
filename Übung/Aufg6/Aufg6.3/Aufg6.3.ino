#include <Servo.h>

Servo tiltServo;
Servo swingServo;

void setup() {

  Serial.begin(115200);

  tiltServo.attach(7);
  swingServo.attach(6);
}

void loop() {
  int xOut = analogRead(A0);
  int yOut = analogRead(A1);
  printf("xout: %d  ", xOut);
  printf("yout: %d\n", yOut);

  xOut = map(xOut, 0, 1023, 0, 180);
  yOut = map(yOut, 0, 1023, 0, 180);
  tiltServo.write(xOut);
  swingServo.write(yOut);
  delay(20);
}
