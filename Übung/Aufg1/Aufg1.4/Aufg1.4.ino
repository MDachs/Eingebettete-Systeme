int led_state = 0;


void setup() {
  pinMode(7, OUTPUT); //LED Output
  pinMode(3, INPUT); //Rechter Taster
  attachInterrupt(digitalPinToInterrupt(3), toggle, FALLING);  
}

void loop() {
}

void toggle(){
    led_state = !led_state;
    digitalWrite(7, led_state);  
}

