#ifndef BUZZER_H
#define BUZZER_H
#include <Arduino.h>

void beepArmed(byte buzzer);
void beepDisarmed(byte buzzer);
void beepError(byte buzzer);
void beepOn(byte buzzer);
void beepOff(byte buzzer);

#endif
