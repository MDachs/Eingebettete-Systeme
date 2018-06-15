

void setup() {

 Serial.begin(115200);
}

void loop() {
  int xOut = analogRead(A0);
  int yOut = analogRead(A1);
  printf("xout: %d  ", xOut);
  printf("yout: %d\n", yOut);
}
