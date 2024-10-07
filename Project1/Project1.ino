#include <Keypad.h>
#include <LiquidCrystal.h>
#include "display.h"

// -------------- Keypad

const byte ROWS = 4; 
const byte COLS = 4; 

char hexaKeys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'}
};

byte rowPins[ROWS] = {2, 3, 4}; 
byte colPins[COLS] = {5, 6, 7}; 

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

// -------------- End: Keypad

// -------------- Password

const int passwordLength = 4;
const char password[passwordLength] = "1111";
char userInput[passwordLength];
int inputCount = 0;

// -------------- End: Password

// -------------- Display
const int rs = 14, en = 9, d4 = 10, d5 = 11, d6 = 12, d7 = 13;
char latestFirstLine[16];
char latestSecondLine[16];
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// -------------- End: Display

// -------------- Alarm
int isArmed = 0;
int armingInProgress = 0;
const int armingCountDownDuration = 10;
int armingCountDown = 0;
int isSirenOn = 0;
// -------------- End: Alarm

const byte greenLed = 19;
const byte redLed = 18;
const byte sensor = 8;
const byte buzzer = 17;

const int logToSerial = 0;

void setup() {
  Serial.begin(9600);
  pinMode(greenLed, OUTPUT);
  pinMode(redLed, OUTPUT);
  pinMode(sensor, INPUT);
  pinMode(buzzer, OUTPUT);

  clearInput();
  setDisarmed();

  analogWrite(15,100); // Generate PWM signal at pin D11, value of 100 (out of 255)

  lcd.begin(16, 2);
}
  
void loop() {

  if (armingInProgress) {
    handleArming();
  } else {
    if (isArmed) {
      checkMotion();
      sirenIfOn();
    }

    handleKeypad();
  }

  displayOutput(lcd, isArmed, armingInProgress, armingCountDown, inputCount);
}

void displayOutput() {
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

  if (strcmp(firstLine, latestFirstLine) != 0 || strcmp(secondLine, latestSecondLine) != 0) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(firstLine);
    lcd.setCursor(0, 1);
    lcd.write(secondLine);
    
    strcpy(latestFirstLine, firstLine);
    strcpy(latestSecondLine, secondLine);
  }

  
}

void handleKeypad() {
  char customKey = customKeypad.getKey();
    
  if (customKey) {
    beepShort();
    userInput[inputCount++] = customKey;

    if (inputCount == passwordLength) {
      userInput[inputCount] = '\0';
      if (isCorrectPassword()) {
        serial("Correct");
        toggleAlarm();
      } else {
        serial("Incorrect");
        beepError();
      }

      clearInput();
    }
  }
}

void checkMotion() {
  if (isSirenOn)
    return;

  serial("Checking motion");
  
  int motion = digitalRead(sensor);
  serial(motion);

  if (motion == HIGH) {
    setSirenOn();
  }
}

void sirenIfOn() {
  if (isSirenOn) {
    beepOn();
    serial("Siren!");
  } else {
    beepOff();
  }
}

void setSirenOn() {
  isSirenOn = true;
  serial("Siren on!");
}

int isCorrectPassword() {
  serial(userInput);
  serial(password);
  serial(strcmp(userInput, password));
  
  return strcmp(userInput, password) == 0;
}

void clearInput() {
  for (int i = 0; i < passwordLength; i++) {
    userInput[i] = "*";
  }
  inputCount = 0;
}

void toggleAlarm() {
  if (isArmed) {
    setDisarmed();
  } else {
    setArminCountDown();
  }
}

void setArminCountDown() {
  armingInProgress = 1;
  armingCountDown = armingCountDownDuration;
}

void setRed() {
  serial("Red");
  digitalWrite(greenLed, LOW);
  digitalWrite(redLed, HIGH);
}

void redOn() {
  digitalWrite(redLed, HIGH);
}

void redOff() {
  digitalWrite(redLed, LOW);
}

void setGreen() {
  serial("Green");
  digitalWrite(greenLed, HIGH);
  digitalWrite(redLed, LOW);
}

void beep() {
  tone(buzzer, 5000, 200);
  serial("beep");
}

void beepShort() {
  tone(buzzer, 2000, 100);
  serial("beep");
}

void handleArming() {
  if (armingCountDown == 0) {
    setArmed();
  } else {
    if (armingCountDown <= 5) {
      redOn();
      beepOn();
      delay(200);
      redOff();
      beepOff();
      delay(500);
      redOn();
      beepOn();
      delay(200);
      redOff();
      beepOff();
      delay(500);
    } else {
      redOn();
      beepOn();
      delay(200);
      redOff();
      beepOff();

      delay(800);
    }

    armingCountDown--;
  }
}

void setDisarmed() {
  isArmed = false;
  isSirenOn = false;
  setGreen();
  beepDisarmed();
}

void setArmed() {
  armingInProgress = 0;
  isArmed = true;
  setRed();
  beepArmed();
  delay(1000);
}

void beepArmed() {
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

void beepDisarmed() {
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

void beepOn() {
  tone(buzzer, 2000);
}

void beepOff() {
  noTone(buzzer);
}

void beepError() {
  noTone(buzzer);
  tone(buzzer, 1000);
  delay(100);
  noTone(buzzer);
  delay(100);
  tone(buzzer, 1000);
  delay(300);
  noTone(buzzer);

}

void serial(char *str) {
  if (logToSerial) {
    Serial.println(str);
  }
}