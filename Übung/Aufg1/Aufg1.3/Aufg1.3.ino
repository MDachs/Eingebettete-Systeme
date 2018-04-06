int last_state = 1;
int led_state = 0;


void setup() {
  pinMode(7, OUTPUT); //LED Output
  pinMode(3, INPUT); //Rechter Taster
}

void loop() {
  int new_state = digitalRead(3);
  if (last_state == 1 && new_state == 0){
    led_state = !led_state;
    digitalWrite(7, led_state);
  }

  last_state = new_state;
  delay(10);
}

