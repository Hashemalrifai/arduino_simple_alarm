#include <Keypad.h>
#include <LiquidCrystal.h>
#include "buzzer.h"
#include <arduino.h>

// -------------- Keypad

const byte ROWS = 4;
const byte COLS = 4;

char hexaKeys[ROWS][COLS] = {
  { '1', '2', '3' },
  { '4', '5', '6' },
  { '7', '8', '9' }
};

byte rowPins[ROWS] = { 2, 3, 4 };
byte colPins[COLS] = { 5, 6, 7 };

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
const int armingCountDownDuration = 10;
// -------------- End: Alarm

struct {
  int isArmed = 0;
  int armingInProgress = 0;
  int armingCountDown = 0;
  int isSirenOn = 0;
  char errorMessage[16] = "";
} status;


struct {
  byte greenLed = 19;
  byte redLed = 18;
  byte sensor = 8;
  byte buzzer = 17;
} io;

const int logToSerial = 0;

void setup() {
  Serial.begin(9600);
  pinMode(io.greenLed, OUTPUT);
  pinMode(io.redLed, OUTPUT);
  pinMode(io.sensor, INPUT);
  pinMode(io.buzzer, OUTPUT);

  clearInput();
  setDisarmed();

  analogWrite(15, 100);  // Generate PWM signal at pin D11, value of 100 (out of 255)

  lcd.begin(16, 2);
}

void loop() {

  if (status.armingInProgress) {
    handleArming();
  } else {
    if (status.isArmed) {
      checkMotion();
      sirenIfOn();
    }

    handleKeypad();
  }

  displayOutput();
}

void displayOutput() {
  char firstLine[16] = "";
  char secondLine[16] = "";

  if (status.armingInProgress) {
    strcpy(firstLine, "Arming in..");
    snprintf(secondLine, 16, "%d seconds!", status.armingCountDown);
  } else if (status.isArmed) {
    strcpy(firstLine, "Armed");
  } else {
    strcpy(firstLine, "Disarmed");
  }

  if (strcmp(status.errorMessage, "") != 0) { // has error
    strcpy(secondLine, status.errorMessage);
  } else if (inputCount > 0) {
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
    tone(io.buzzer, 2000, 100);
    userInput[inputCount++] = customKey;

    if (inputCount == passwordLength) {
      userInput[inputCount] = '\0';
      if (isCorrectPassword()) {
        toggleAlarm();
      } else {
        beepError(io.buzzer);
        strcpy(status.errorMessage, "Invalid PIN");
      }

      clearInput();
    } else {
      strcpy(status.errorMessage, "");
    }
  }
}

void checkMotion() {
  if (status.isSirenOn)
    return;

  int motion = digitalRead(io.sensor);

  if (motion == HIGH) {
    setSirenOn();
  }
}

void sirenIfOn() {
  if (status.isSirenOn) {
    beepOn(io.buzzer);
  } else {
    beepOff(io.buzzer);
  }
}

void setSirenOn() {
  status.isSirenOn = true;
}

int isCorrectPassword() {
  return strcmp(userInput, password) == 0;
}

void clearInput() {
  for (int i = 0; i < passwordLength; i++) {
    userInput[i] = "*";
  }
  inputCount = 0;
}

void toggleAlarm() {
  if (status.isArmed) {
    setDisarmed();
  } else {
    setArminCountDown();
  }
}

void setArminCountDown() {
  status.armingInProgress = 1;
  status.armingCountDown = armingCountDownDuration;
}

void setRed() {
  digitalWrite(io.greenLed, LOW);
  digitalWrite(io.redLed, HIGH);
}

void redOn() {
  digitalWrite(io.redLed, HIGH);
}

void redOff() {
  digitalWrite(io.redLed, LOW);
}

void setGreen() {
  digitalWrite(io.greenLed, HIGH);
  digitalWrite(io.redLed, LOW);
}

void handleArming() {
  if (status.armingCountDown == 0) {
    setArmed();
  } else {
    if (status.armingCountDown <= 5) {
      redOn();
      beepOn(io.buzzer);
      delay(200);
      redOff();
      beepOff(io.buzzer);
      delay(500);
      redOn();
      beepOn(io.buzzer);
      delay(200);
      redOff();
      beepOff(io.buzzer);
      delay(500);
    } else {
      redOn();
      beepOn(io.buzzer);
      delay(200);
      redOff();
      beepOff(io.buzzer);

      delay(800);
    }

    status.armingCountDown--;
  }
}

void setDisarmed() {
  status.isArmed = false;
  status.isSirenOn = false;
  setGreen();
  beepDisarmed(io.buzzer);
}

void setArmed() {
  status.armingInProgress = 0;
  status.isArmed = true;
  setRed();
  beepArmed(io.buzzer);
  delay(1000);
}
