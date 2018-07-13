//Speichern
int last1State = HIGH;
int last1Count = 0;

int stateFirst = HIGH;


void entprellen(void)
{
  int new1State = digitalRead(6);
  if (new1State != last1State) {
    //State changed
    last1Count = 0;
  }

  if (last1Count <= 50) {
    last1Count++;
  }
  if (last1Count == 50) {
    if (new1State == HIGH) {
      firstReleased();
    } else {
      firstPressed();
    }
  }

  last1State = new1State;
}

//Speichern
void firstReleased() {
  static bool first = true;
  if (first) {
    first = false;
    return;
  }

  Serial.println("Saving");

  stateFirst = HIGH;

  //Do stuff

}


void firstPressed() {
  stateFirst = LOW;
}

