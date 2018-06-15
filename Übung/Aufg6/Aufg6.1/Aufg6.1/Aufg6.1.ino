void setup() {
 pinMode(A0, INPUT); //muss man die analogen Pins setzten?
 pinMode(A1, INPUT);

 Serial.begin(9600)
}

void loop() {
  int xOut = analogRead(A0);
  int yOut = analogRead(A1);
  Serial.println("xout: %d", xOut);
  Serial.println("yout: %d", tOut);
}
