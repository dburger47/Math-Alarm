# Math-Alarm
uCtrl Fall 2024 Final Project

Welcome,

The math alarm clock uses an Arduino, Real Time Clock Module, 4x4 keypad matrix, and a 2x16 LCD screen. Besides Arduino code, the LiquidCrystal_I2C.h and uRTCLib.h libraries (both found in Arduino library manager) are required for I2C communication to the RTC and LCD. This clock functions like a normal clock, but to stop the alarm, the user must answer a few math problems. The math problems are displayed in multiple-choice format, meaning the user must select the correct problem that gives a set answer. 

ex.)

Ans: 6

A: 5 + 8 - 7

B: 10 - 8 + 3

To reset the current time, press the "D" key and enter the time in hours and mins.

To set the alarm time, press the "A" button and enter the time hours and mins.

To toggle on and off the alarm, press the "B" button.
