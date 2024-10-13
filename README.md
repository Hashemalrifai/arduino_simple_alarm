# Arduino project

## Simple Security System

Motion detection alarm system.

## Features

- Arm and disarm using a matrix keypad: The user types a pin code to arm or disarm the system. The pin code will be displayed on an LCD display.
- LCD to display user input and system feedback (“Armed”, “Disarmed”, “Incorrect PIN”).
- Led indicator of alarm status, armed (red) or disarmed (green).
- Buzzer for:
  - Counting down sound when arming the system: When armed, the system beeps for 30 (configurable) seconds, then the system starts to monitor.
  - Siren: When the alarm is triggered (motion detected), a siren will start (can be stopped by entering the PIN correctly).

## How it works

The most important thing in this system is the ability to get the status of the system at any time, and based on that status, initiate the correct action. These are the statuses we cover:

System statuses:

- Disarmed: The system is idle and doesn’t monitor any motion.
- Armed: The system is active. Motion detection is on.
- Arming in progress: The user has entered a correct PIN asking the system to arm, but the system will start counting down before switching to arm state.
- Siren: Happens when the system is armed, and motion has been detected.
- Error? Error message: If there is any error to display on the screen. (ex. incorrect PIN).

System inputs components:

- Matrix keypad.
- PIR motion sensor.

System outputs components:

- Liquid LCD display.
- LEDs (green, red).
- Buzzer/Piezo (feedback sounds, and siren).

Configurable constants:

- Password length.
- Password.

## Flow chart

![Flowchart](https://github.com/user-attachments/assets/f72e3549-251f-4274-8065-14fb5f8897d3)

