#include "buzzer.h"
#include <Arduino.h>

void beepOn(byte buzzer) {
  tone(buzzer, 2000);
}

void beepOff(byte buzzer) {
  noTone(buzzer);
}

void beepArmed(byte buzzer) {
  const int duration = 100;
  noTone(buzzer);
  delay(duration);
  tone(buzzer, 2000);
  delay(duration);
  noTone(buzzer);
  tone(buzzer, 3000);
  delay(duration);
  noTone(buzzer);
  tone(buzzer, 4000);
  delay(duration);
  noTone(buzzer);
}

void beepDisarmed(byte buzzer) {
  const int duration = 100;
  tone(buzzer, 4000);
  delay(duration);
  noTone(buzzer);
  tone(buzzer, 3000);
  delay(duration);
  noTone(buzzer);
  tone(buzzer, 2000);
  delay(duration);
  noTone(buzzer);
}

void beepError(byte buzzer) {
  noTone(buzzer);
  tone(buzzer, 1000);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 1000);
  delay(300);
  noTone(buzzer);
}
