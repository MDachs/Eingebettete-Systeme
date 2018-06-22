//Laser an aus
int last1State = HIGH;
int last1Count = 0;
//Wiedergabe
int last2State = HIGH;
int last2Count = 0;
//Joystick
int last3State = HIGH;
int last3Count = 0;

int stateFirst = HIGH;
int stateSecond = HIGH;
int stateThird = HIGH;

bool block = false;

struct pos {
  int x;
  int y;
} posList[10];

int listSize = 0;

void entprellen(void)
{
  int new1State = digitalRead(31);
  int new2State = digitalRead(43);
  int new3State = digitalRead(47);
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

  if (new2State != last2State) {
    // State Changed
    last2Count = 0;
  }

  if (last2Count <= 50) {
    last2Count++;
  }
  if (last2Count == 50) {
    if (new2State == HIGH) {
      secondReleased();
    } else {
      secondPressed();
    }
  }

  if (new3State != last3State) {
    // State Changed
    last3Count = 0;
  }

  if (last3Count <= 50) {
    last3Count++;
  }
  if (last3Count == 50) {
    if (new3State == HIGH) {
      thirdReleased();
    } else {
      thirdPressed();
    }
  }

  last1State = new1State;
  last2State = new2State;
  last3State = new3State;
}

void firstReleased() {
  stateFirst = HIGH;

  static bool ledState = true;
  ledState = !ledState;
  digitalWrite(26, ledState);

}

void secondReleased() {
  stateSecond = HIGH;

  if (block == true) {
    block = false;
    return;
  }

  if (stateFirst == LOW) {
    //changeState();
    block = true;
    return;
  }

  playing = 1;

}

void thirdReleased() {
  stateThird = HIGH;

  if (block == true) {
    block = false;
    return;
  }

  if (stateFirst == LOW) {
    //changeState();
    block = true;
    return;
  }

  if (listSize < 10) {
    posList[listSize++] = {xpos, ypos};
  }

}

void firstPressed() {
  stateFirst = LOW;
}

void secondPressed() {
  stateSecond = LOW;
}

void thirdPressed() {
  stateThird = LOW;
}

