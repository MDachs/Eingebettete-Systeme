#include <DueTimer.h>
DueTimer timer;

const int REDPIN = 9;
const int GREENPIN = 8;
const int BLUEPIN = 7;

const int BTN2PIN = 31;
const int BTN1PIN = 43;

const int PWMPIN = 6;
const int IN1PIN = 47;
const int IN2PIN = 49;

const int POWER = 1;
const int DIRECTION = 0;
int settingState = DIRECTION;

int powerState = 0;

bool block = false;

const int CW = 0;
const int CCW = 1;
const int STOP = 2;
int directionState = STOP;

int stateFirst = HIGH;
int stateSecond = HIGH;

int last1State = HIGH;
int last1Count = 0;
int last2State = HIGH;
int last2Count = 0;

void setup() {
  Serial.begin(9600);

  pinMode(BTN1PIN, INPUT_PULLUP);
  pinMode(BTN2PIN, INPUT_PULLUP);
  setRGB(0.0, 1.0, 0.0);

  if (timer.configure(1000, entprellen))
  {
    timer.start();
  }

  delay(100);
  printStatus();
}

void loop() {
  analogWrite(PWMPIN, (255.0 / 100) * powerState);
}



void entprellen(void)
{
  int new1State = digitalRead(BTN1PIN);
  int new2State = digitalRead(BTN2PIN);
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

  last1State = new1State;
  last2State = new2State;
}


void firstReleased() {
  stateFirst = HIGH;

  if (block == true) {
    block = false;
    return;
  }

  if (stateSecond == LOW) {
    changeState();
    block = true;
    return;
  }

  if (settingState == DIRECTION) {
    switch (directionState) {
      case CW:
        directionState = STOP;
        powerState = 0;
        printStatus();
        break;
      case STOP:
        directionState = CCW;
        digitalWrite(IN1PIN, LOW);
        digitalWrite(IN2PIN, HIGH);
        printStatus();
        break;
    }
  } else {
    if (powerState < 97) {
      powerState += 3;
    }

    printStatus();
  }
}

void secondReleased() {
  stateSecond = HIGH;

  if (block == true) {
    block = false;
    return;
  }

  if (stateFirst == LOW) {
    changeState();
    block = true;
    return;
  }

  if (settingState == DIRECTION) {
    switch (directionState) {
      case CCW:
        directionState = STOP;
        powerState = 0;
        printStatus();
        break;
      case STOP:
        directionState = CW;
        digitalWrite(IN1PIN, HIGH);
        digitalWrite(IN2PIN, LOW);
        printStatus();
        break;
    }
  } else {
    if (powerState > 3) {
      powerState -= 3;
    }
    printStatus();
  }
}

void firstPressed() {
  stateFirst = LOW;
}

void secondPressed() {
  stateSecond = LOW;
}

void changeState() {
  settingState = !settingState;
  if (settingState == DIRECTION) {
    setRGB(0.0, 1.0, 0.0);
  } else {
    setRGB(1.0, 0.0, 0.0);
  }
  printStatus();
}

void setRGB(float red, float green, float blue) {
  if ( red > 1.0 || red < 0.0) {
    Serial.println("Error: Argument  1 value must be within range 0.0 and 1.0");
    return;
  }
  if ( green > 1.0 || green < 0.0) {
    Serial.println("Error: Argument  2 value must be within range 0.0 and 1.0");
    return;
  }
  if ( blue > 1.0 || blue < 0.0) {
    Serial.println("Error: Argument  3 value must be within range 0.0 and 1.0");
    return;
  }

  analogWrite(REDPIN, (255.0 * (1 - red)));
  analogWrite(GREENPIN, (255.0 * (1 - green)));
  analogWrite(BLUEPIN, (255.0 * (1 - blue)));
}

void printStatus() {
  String ccwString =  (directionState == CCW) ? "(CCW)" : "CCW";
  String stopString =  (directionState == STOP) ? "(STOP)" : "STOP";
  String cwString =  (directionState == CW) ? "(CW)" : "CW";

  String mode = (settingState == POWER) ? "POWER" : "DIRECTION";

  printf("[%s MODE] Motor direction: %s - %s - %s | Motor Power: %d% \n", mode.c_str(), ccwString.c_str(), stopString.c_str(), cwString.c_str(), powerState);
}

