int currentPos = 0;
int currentStep = 0;

int xstart = 0;
int ystart = 0;

void play(){
  if (playing == 1){
    if (currentPos >= listSize){
      playing = 0;
      currentPos = 0;
      currentStep = 0;
      return;
    }

    if (currentStep == 0){
      xstart = xpos;
      ystart = ypos;
    }

    xpos = xstart + (int)(((float)(posList[currentPos].x - xstart) / 100.0) * (float)currentStep+1);
    ypos = ystart + (int)(((float)(posList[currentPos].y - ystart) / 100.0) * (float)currentStep+1);

    currentStep++;
    if (currentStep > 99){
      currentStep = 0;
      currentPos++;
    }
  }
}

