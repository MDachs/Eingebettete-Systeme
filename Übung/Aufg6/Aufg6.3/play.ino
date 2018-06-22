

int currentPos = 0;
int currentStep = 0;
int ysize = 0;
int xsize = 0;

void play(){
  if (playing == 1){
    if (currentPos > 9){
      playing = 0;
      currentPos = 0;
      currentStep = 0;
      return;
    }

    if (currentStep == 0){
      xsize = (posList[currentPos].x - xpos) / 100;
      ysize = (posList[currentPos].y - ypos) / 100;
    }

    xpos += xsize;
    ypos += ysize;

    currentStep++;
    if (currentStep > 99){
      currentStep = 0;
      currentPos++;
    }
  }
}

