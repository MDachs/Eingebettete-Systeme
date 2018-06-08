#include <SPI.h>
#include <DueTimer.h>
#include <SD.h>

DueTimer timer;
DueTimer timer2;

const int PIN_RST = 9;
const int PIN_CS = 10; // SCE PIN
const int PIN_DC = 3;

const int PIN_SD = 4;

uint8_t buffer[6 * 84];

unsigned char font[95][6] =
{
  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }, // space
  { 0x00, 0x00, 0x5F, 0x00, 0x00, 0x00 }, // !
  { 0x00, 0x07, 0x00, 0x07, 0x00, 0x00 }, // "
  { 0x14, 0x7F, 0x14, 0x7F, 0x14, 0x00 }, // #
  { 0x24, 0x2A, 0x7F, 0x2A, 0x12, 0x00 }, // $
  { 0x23, 0x13, 0x08, 0x64, 0x62, 0x00 }, // %
  { 0x36, 0x49, 0x55, 0x22, 0x50, 0x00 }, // &
  { 0x00, 0x00, 0x07, 0x00, 0x00, 0x00 }, // '
  { 0x00, 0x1C, 0x22, 0x41, 0x00, 0x00 }, // (
  { 0x00, 0x41, 0x22, 0x1C, 0x00, 0x00 }, // )
  { 0x0A, 0x04, 0x1F, 0x04, 0x0A, 0x00 }, // *
  { 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00 }, // +
  { 0x00, 0x50, 0x30, 0x00, 0x00, 0x00 }, // ,
  { 0x08, 0x08, 0x08, 0x08, 0x08, 0x00 }, // -
  { 0x00, 0x60, 0x60, 0x00, 0x00, 0x00 }, // .
  { 0x20, 0x10, 0x08, 0x04, 0x02, 0x00 }, // slash
  { 0x3E, 0x51, 0x49, 0x45, 0x3E, 0x00 }, // 0
  { 0x00, 0x42, 0x7F, 0x40, 0x00, 0x00 }, // 1
  { 0x42, 0x61, 0x51, 0x49, 0x46, 0x00 }, // 2
  { 0x21, 0x41, 0x45, 0x4B, 0x31, 0x00 }, // 3
  { 0x18, 0x14, 0x12, 0x7F, 0x10, 0x00 }, // 4
  { 0x27, 0x45, 0x45, 0x45, 0x39, 0x00 }, // 5
  { 0x3C, 0x4A, 0x49, 0x49, 0x30, 0x00 }, // 6
  { 0x03, 0x71, 0x09, 0x05, 0x03, 0x00 }, // 7
  { 0x36, 0x49, 0x49, 0x49, 0x36, 0x00 }, // 8
  { 0x06, 0x49, 0x49, 0x29, 0x1E, 0x00 }, // 9
  { 0x00, 0x36, 0x36, 0x00, 0x00, 0x00 }, // :
  { 0x00, 0x56, 0x36, 0x00, 0x00, 0x00 }, // ;
  { 0x08, 0x14, 0x22, 0x41, 0x00, 0x00 }, // <
  { 0x14, 0x14, 0x14, 0x14, 0x14, 0x00 }, // =
  { 0x00, 0x41, 0x22, 0x14, 0x08, 0x00 }, // >
  { 0x02, 0x01, 0x51, 0x09, 0x06, 0x00 }, // ?
  { 0x32, 0x49, 0x79, 0x41, 0x3E, 0x00 }, // @
  { 0x7E, 0x11, 0x11, 0x11, 0x7E, 0x00 }, // A
  { 0x7F, 0x49, 0x49, 0x49, 0x36, 0x00 }, // B
  { 0x3E, 0x41, 0x41, 0x41, 0x22, 0x00 }, // C
  { 0x7F, 0x41, 0x41, 0x41, 0x3E, 0x00 }, // D
  { 0x7F, 0x49, 0x49, 0x49, 0x41, 0x00 }, // E
  { 0x7F, 0x09, 0x09, 0x09, 0x01, 0x00 }, // F
  { 0x3E, 0x41, 0x41, 0x49, 0x7A, 0x00 }, // G
  { 0x7F, 0x08, 0x08, 0x08, 0x7F, 0x00 }, // H
  { 0x00, 0x41, 0x7F, 0x41, 0x00, 0x00 }, // I
  { 0x20, 0x40, 0x41, 0x3F, 0x01, 0x00 }, // J
  { 0x7F, 0x08, 0x14, 0x22, 0x41, 0x00 }, // K
  { 0x7F, 0x40, 0x40, 0x40, 0x40, 0x00 }, // L
  { 0x7F, 0x02, 0x0C, 0x02, 0x7F, 0x00 }, // M
  { 0x7F, 0x04, 0x08, 0x10, 0x7F, 0x00 }, // N
  { 0x3E, 0x41, 0x41, 0x41, 0x3E, 0x00 }, // O
  { 0x7F, 0x09, 0x09, 0x09, 0x06, 0x00 }, // P
  { 0x3E, 0x41, 0x51, 0x21, 0x5E, 0x00 }, // Q
  { 0x7F, 0x09, 0x19, 0x29, 0x46, 0x00 }, // R
  { 0x26, 0x49, 0x49, 0x49, 0x32, 0x00 }, // S
  { 0x01, 0x01, 0x7F, 0x01, 0x01, 0x00 }, // T
  { 0x3F, 0x40, 0x40, 0x40, 0x3F, 0x00 }, // U
  { 0x1F, 0x20, 0x40, 0x20, 0x1F, 0x00 }, // V
  { 0x3F, 0x40, 0x38, 0x40, 0x3F, 0x00 }, // W
  { 0x63, 0x14, 0x08, 0x14, 0x63, 0x00 }, // X
  { 0x07, 0x08, 0x70, 0x08, 0x07, 0x00 }, // Y
  { 0x61, 0x51, 0x49, 0x45, 0x43, 0x00 }, // Z
  { 0x00, 0x7F, 0x41, 0x41, 0x00, 0x00 }, // [
  { 0x02, 0x04, 0x08, 0x10, 0x20, 0x00 }, // backslash
  { 0x00, 0x41, 0x41, 0x7F, 0x00, 0x00 }, // ]
  { 0x04, 0x02, 0x01, 0x02, 0x04, 0x00 }, // ^
  { 0x40, 0x40, 0x40, 0x40, 0x40, 0x00 }, // _
  { 0x00, 0x01, 0x02, 0x04, 0x00, 0x00 }, // `
  { 0x20, 0x54, 0x54, 0x54, 0x78, 0x00 }, // a
  { 0x7F, 0x48, 0x44, 0x44, 0x38, 0x00 }, // b
  { 0x38, 0x44, 0x44, 0x44, 0x20, 0x00 }, // c
  { 0x38, 0x44, 0x44, 0x48, 0x7F, 0x00 }, // d
  { 0x38, 0x54, 0x54, 0x54, 0x18, 0x00 }, // e
  { 0x08, 0x7E, 0x09, 0x01, 0x02, 0x00 }, // f
  { 0x08, 0x54, 0x54, 0x54, 0x3C, 0x00 }, // g
  { 0x7F, 0x08, 0x04, 0x04, 0x78, 0x00 }, // h
  { 0x00, 0x48, 0x7D, 0x40, 0x00, 0x00 }, // i
  { 0x20, 0x40, 0x44, 0x3D, 0x00, 0x00 }, // j
  { 0x7F, 0x10, 0x28, 0x44, 0x00, 0x00 }, // k
  { 0x00, 0x41, 0x7F, 0x40, 0x00, 0x00 }, // l
  { 0x7C, 0x04, 0x78, 0x04, 0x78, 0x00 }, // m
  { 0x7C, 0x08, 0x04, 0x04, 0x78, 0x00 }, // n
  { 0x38, 0x44, 0x44, 0x44, 0x38, 0x00 }, // o
  { 0x7C, 0x14, 0x14, 0x14, 0x08, 0x00 }, // p
  { 0x08, 0x14, 0x14, 0x18, 0x7C, 0x00 }, // q
  { 0x7C, 0x08, 0x04, 0x04, 0x08, 0x00 }, // r
  { 0x48, 0x54, 0x54, 0x54, 0x20, 0x00 }, // s
  { 0x04, 0x3F, 0x44, 0x40, 0x20, 0x00 }, // t
  { 0x3C, 0x40, 0x40, 0x20, 0x7C, 0x00 }, // u
  { 0x1C, 0x20, 0x40, 0x20, 0x1C, 0x00 }, // v
  { 0x3C, 0x40, 0x30, 0x40, 0x3C, 0x00 }, // w
  { 0x44, 0x28, 0x10, 0x28, 0x44, 0x00 }, // x
  { 0x0C, 0x50, 0x50, 0x50, 0x3C, 0x00 }, // y
  { 0x44, 0x64, 0x54, 0x4C, 0x44, 0x00 }, // z
  { 0x00, 0x08, 0x36, 0x41, 0x00, 0x00 }, // {
  { 0x00, 0x00, 0x7F, 0x00, 0x00, 0x00 }, // |
  { 0x00, 0x41, 0x36, 0x08, 0x00, 0x00 }, // }
  { 0x10, 0x08, 0x08, 0x10, 0x08, 0x00 } // ~
};

void setup() {
  // Display initialisieren
  pinMode(PIN_RST, OUTPUT);
  pinMode(PIN_DC, OUTPUT);

  SPI.begin(PIN_CS);
  SD.begin(PIN_SD);
  Serial.begin(115200);

  // Reset durchführen
  digitalWrite(PIN_RST, LOW);
  delay(500);
  digitalWrite(PIN_RST, HIGH);
  delay(500);

  //Datenübertragung initialisieren
  digitalWrite(PIN_DC, LOW);
  SPI.beginTransaction(PIN_CS, SPISettings(1000000, MSBFIRST, SPI_MODE0));
  SPI.transfer(PIN_CS, 0x21);// FUNCTION SET (extended instruction set)
  SPI.transfer(PIN_CS, 0x13);// SET BIAS (1:48 MUX, 1/8 BIAS)
  SPI.transfer(PIN_CS, 0xC0);// SET CONTRAST (V_OP = 6,18 V)
  SPI.transfer(PIN_CS, 0x04);// SET TEMPERATURE COEFFICIENT
  SPI.transfer(PIN_CS, 0x20);// FUNCTION SET (normal instruction set)
  SPI.transfer(PIN_CS, 0x0C);// SET DISPLAY MODE (normal)
  SPI.transfer(PIN_CS, 0x40);// SET Y COORDINATE = 0
  SPI.transfer(PIN_CS, 0x80);// SET X COORDINATE = 0
  SPI.endTransaction();

  digitalWrite(PIN_DC, HIGH);
  for (int i = 0; i < 84 * 6; i++) {
    SPI.beginTransaction(PIN_CS, SPISettings(1000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(PIN_CS, 0xFF);
    SPI.endTransaction();
  }
  delay(1000);
  for (int i = 0; i < 84 * 6; i++) {
    SPI.beginTransaction(PIN_CS, SPISettings(1000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(PIN_CS, 0x00);
    SPI.endTransaction();
  }


  if (timer.configure(1, studentDemo)) {
    timer.start();
  }
  if (timer2.configure(3, rotatingDemo)) {
    timer2.start();
  }
}


bool runStudent = 0;

void studentDemo() {
  static int wait = 0;
  if (runStudent == true) {
    static bool state = 0;
    if (wait >= 5) {
      if (state == 0) {
        clearDisplay();
        printString(0, 0, "Meik");
        printString(0, 8, "Dachs");
        printString(0, 16, "6005245");
        //updateDisplay();
      } else {
        clearDisplay();
        printString(0, 0, "Alexander");
        printString(0, 8, "Lunge");
        printString(0, 16, "6813791");
        //updateDisplay();
      }
      state = !state;
      wait = 0;
    } else {
      wait++;
    }
  } else {
    wait = 5;
  }
}

bool runRotating = false;

void rotatingDemo() {
  if (runRotating == true) {
    static int state = 0;
    switch (state) {
      case 0:
        printString(0, 16,   "      |     ");
        printString(0, 24,   "      |     ");
        printString(0, 32,   "      |     ");
        break;
      case 1:
        printString(0, 16,   "       /    ");
        printString(0, 24,   "      /     ");
        printString(0, 32,   "     /      ");
        break;
      case 2:
        printString(0, 16,   "            ");
        printString(0, 24,   "     ---    ");
        printString(0, 32,   "            ");
        break;
      case 3:
        printString(0, 16,   "     \\      ");
        printString(0, 24,   "      \\     ");
        printString(0, 32,   "       \\    ");
        break;
      default:
        break;
    }
    state++;
    if (state == 4) {
      state = 0;
    }
  }
}


void loop() {
  for (int i = 0; i < 84 * 6; i++) {
    SPI.beginTransaction(PIN_CS, SPISettings(1000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(PIN_CS, buffer[i]);
    SPI.endTransaction();
  }
  // delay(1);

  while (Serial.available() > 0) {
    char ch = Serial.read();
    //Serial.write(ch);
    parser(ch);
  }

}

void runStudentIdDemo() {
  runStudent = true;
}

void updateDisplay() {
  for (int i = 0; i < 84 * 6; i++) {
    SPI.beginTransaction(PIN_CS, SPISettings(1000000, MSBFIRST, SPI_MODE0));
    SPI.transfer(PIN_CS, buffer[i]);
    SPI.endTransaction();
  }
}

void clearDisplay() {
  for (int i = 0; i < 84 * 6; i++) {
    buffer[i] = 0x00;
  }
}

void setPixel(int x , int y, int value) {
  int ypos = y / 8;
  int pos;
  if (ypos == 0) {
    pos = x;
  } else if (x == 0) {
    pos = ypos * 84;
  } else {
    pos = x + (ypos * 84);
  }

  if (value == 1) {
    buffer[pos] |= 1 << (y % 8);
  } else {
    buffer[pos] &= ~(1 << (y % 8));
  }
  //printf("x: %d, y: %d, ypos: %d, pos: %d, buf: %x\n", x, y, ypos, pos, buffer[pos]);
}

int printChar(int x, int y, char value) {
  if (y < 0 || y > 39 || x < 0 || x > 77) {
    return -1;
  }

  for (int i = 0; i < 6; i++) {
    for (int j = 0; j < 8; j++) {
      int val = font[value - 32][i] & (1 << j);
      int val2 = 0;
      if (val > 0) {
        val2 = 1;
      }
      setPixel(x + i, y + j, val2);
      //printf("x: %d, y: %d, val2: %d\n", x+i, x+j, val2);
    }
  }
}

int printString(int x, int y, char* c_str) {
  char * t;
  int i = 0;
  for (t = c_str; *t != '\0'; t++) {
    if (*t == '\n' || i > 13) {
      y += 8;
      continue;
    }
    if (printChar(x + i * 6, y, *t) == -1) {
      return -1;
    }
    i++;
  }
}

void parser(char newChar) {
  static String currentCommand;
  currentCommand += newChar;

  if (newChar == '\n') {
    Serial.println("INPUT: " + currentCommand);

    //Befehl verarbeiten
    int firstBracket = currentCommand.indexOf("(");
    int lastBracket = currentCommand.indexOf(")");
    String befehl = currentCommand.substring(0, firstBracket);
    String parameter = currentCommand.substring(firstBracket + 1, lastBracket);

    Serial.println("CMD: " + befehl);

    float parameterarray[10];
    String paraStringarray[10];
    int parametercount = 0;
    int comma = parameter.indexOf(",");
    while (parameter.length() > 0) {
      String parameterstring = parameter.substring(0, comma);
      parameterarray[parametercount] = parameterstring.toFloat();
      paraStringarray[parametercount] = parameterstring;

      Serial.println("ARG_" + String(parametercount) + ":" + String(parameterarray[parametercount]));

      if (comma != -1) {
        parameter = parameter.substring(comma + 1);
      } else {
        parameter = "";
      }
      comma = parameter.indexOf(",");
      parametercount++;
    }

    if (befehl == "help" && parametercount == 0) {
      Serial.println("Befehle:");
      Serial.println("help()");
      Serial.println("setContrast(value)");
      Serial.println("clearDisplay()");
      Serial.println("runRotatingBarDemo()");
      Serial.println("runStudentIdDemo()");
      Serial.println("stopDemo()");
    }
    else if (befehl == "setContrast" && parametercount == 1) {
      float con = parameterarray[0] * 100.0;
      float contrast = (127.0 / 100.0) * con;
      digitalWrite(PIN_DC, LOW);
      SPI.beginTransaction(PIN_CS, SPISettings(1000000, MSBFIRST, SPI_MODE0));
      SPI.transfer(PIN_CS, 0x21);// FUNCTION SET (extended instruction set)
      SPI.transfer(PIN_CS, 128 + contrast); // SET CONTRAST (V_OP = 6,18 V)
      SPI.endTransaction();

      digitalWrite(PIN_DC, HIGH);
    }
    else if (befehl == "clearDisplay" && parametercount == 0) {
      clearDisplay();
    }
    else if (befehl == "runRotatingBarDemo" && parametercount == 0) {
      runRotating = true;
    }
    else if (befehl == "runStudentIdDemo" && parametercount == 0) {
      runStudentIdDemo();
    }
    else if (befehl == "stopDemo" && parametercount == 0) {
      runStudent = false;
      runRotating = false;
      clearDisplay();
    } else if (befehl == "listDirectory" && parametercount == 1) {
      String arg = paraStringarray[0];
      File loc = SD.open(arg);
      while (true) {
        File entry =  loc.openNextFile();
        if (! entry) {
          // no more files
          break;
        }
        Serial.print(entry.name());
        if (entry.isDirectory()) {
          Serial.println("/");

        } else {
          // files have sizes, directories do not
          Serial.print(" ");
          Serial.println(entry.size(), DEC);
        }
        entry.close();
      }

    } else if (befehl == "doesFileExists" && parametercount == 1) {
      String arg = paraStringarray[0];
      File loc = SD.open(arg);
      if (!loc) {
        Serial.println(arg + " :Existiert nicht");
      } else {
        Serial.println(arg + " Existiert");
      }
      loc.close();

    } else if (befehl == "outputFileToSerial" && parametercount == 1) {
      String arg = paraStringarray[0];
      File loc = SD.open(arg);
      if (!loc) {
        Serial.println(arg + " :Existiert nicht");
      } else {
        while (loc.available()) {
          Serial.write(loc.read());
        }
        // close the file:
        loc.close();
      }

    } else if (befehl == "outputFileToLCD" && parametercount == 1) {
      String arg = paraStringarray[0];
      File loc = SD.open(arg);
      if (!loc) {
        Serial.println(arg + " :Existiert nicht");
      } else {
        if (arg.endsWith(".TXT")) {
          char tmp2[4000];
          char *ptr = &tmp2[0];
          while (loc.available()) {
            loc.read(ptr, 1);
            ptr++;
          }

          if (printString(0, 0, tmp2) == -1) {
            clearDisplay();
            printString(0, 0, "TXT_SIZE_ERR");
          }
        }
        if (arg.endsWith(".IMG")) {
          char tmp2[4000];
          char *ptr = &tmp2[0];
          while (loc.available()) {
            loc.read(ptr, 1);
            ptr++;
          }

          String str(tmp2);
          char* tmp1 = strtok(tmp2, "\n");
          char* picsize = &(*tmp1);
          tmp1 = strtok (NULL, "\n");
          char* data = &(*tmp1);

          int xSize = atoi(strtok(picsize, ","));
          int ySize = atoi(strtok(NULL, ","));

          int x = 0;
          int y = 0;
          
          tmp1 = strtok (data, ",");
          while(tmp1 != NULL){
            if (x >= xSize){
              x = 0;
              y++;
            }

            setPixel(x, y, atoi(tmp1));
            
            x++;
            tmp1 = strtok (NULL, ",");
          }

        }
        // close the file:
        loc.close();
      }

    }
    else {
      Serial.println("Unknown Command");
    }

    currentCommand = "";
  }
}

