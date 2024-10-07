#include <LiquidCrystal.h>
#include "display.h"

void displayOutput(LiquidCrystal lcd, int isArmed, int armingInProgress, int armingCountDown, int inputCount) {
  char firstLine[16] = "";
  char secondLine[16] = "";

  if (armingInProgress) {
    strcpy(firstLine, "Arming in..");
    snprintf(secondLine, 16, "%d seconds!", armingCountDown);
  } else if (isArmed) {
    strcpy(firstLine, "Armed");
  } else {
    strcpy(firstLine, "Disarmed");
  }

  if (inputCount > 0) {
    for (int i = 0; i < inputCount; i++) {
      strcat(secondLine, "*");
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.write(firstLine);
  lcd.setCursor(0, 1);
  lcd.write(secondLine);
  delay(50);
}