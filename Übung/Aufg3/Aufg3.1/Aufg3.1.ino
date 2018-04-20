void setup() {
  Serial.begin(9600);

  pinMode(7, OUTPUT);//Blau
  pinMode(8, OUTPUT);//Grün
  pinMode(9, OUTPUT);//ROT

  analogWrite(9, 255);
  analogWrite(8, 255);
  analogWrite(7, 255);

  delay(300);
  Serial.println("------------------------------------------------------------------------");
  Serial.println("To control the LED send the command: 'setRGB(R_value, G_value, B_value)'");
  Serial.println("R, G and B values must be within range of 0.0 and 1.0");

}

void loop() {
  while (Serial.available() > 0) {
    char ch = Serial.read();
    //Serial.write(ch);
    parser(ch);
  }
}

void setRGB(float red, float green, float blue) {
  if ( red > 1.0 || red < 0.0) {
    Serial.println("Error: Argument  1 value must be within range 0.0 and 1.0");
  }
  if ( green > 1.0 || green < 0.0) {
    Serial.println("Error: Argument  2 value must be within range 0.0 and 1.0");
  }
  if ( blue > 1.0 || blue < 0.0) {
    Serial.println("Error: Argument  3 value must be within range 0.0 and 1.0");
  }

  analogWrite(9, (255.0 * (1 - red)));
  analogWrite(8, (255.0 * (1 - green)));
  analogWrite(7, (255.0 * (1 - blue)));
}

//setRGB(0.0,0.0,0.0)
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
    int parametercount = 0;
    int comma = parameter.indexOf(",");
    while (parameter.length() > 0) {
      String parameterstring = parameter.substring(0, comma);
      parameterarray[parametercount] = parameterstring.toFloat();

      Serial.println("ARG_" + String(parametercount) + ":" + String(parameterarray[parametercount]));

      if (comma != -1) {
        parameter = parameter.substring(comma + 1);
      } else {
        parameter = "";
      }
      comma = parameter.indexOf(",");
      parametercount++;
    }

    if (befehl == "setRGB" && parametercount == 3) {
      setRGB(parameterarray[0], parameterarray[1], parameterarray[2]);
    } else {
      Serial.println("Unknown Command");
    }

    currentCommand = "";
  }
}

