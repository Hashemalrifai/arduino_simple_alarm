#ifndef DISPLAY_H
#define DISPLAY_H
#include <Arduino.h>
#include <LiquidCrystal.h>

void displayOutput(LiquidCrystal lcd, int isArmed, int armingInProgress, int armingCountDown, int inputCount);

#endif