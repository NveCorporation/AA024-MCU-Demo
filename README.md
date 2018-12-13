# AA024-MCU-Demo
Interfaces a GMR sensor to an ATtiny85 microcontrollers, which does a simple four-segment piecewise-linear calibration algorithm. The calibrated or uncalibrated outputs are displayed on a digital meter. The microcontroller includes a Trinket bootloader and the convenient Arduino IDE for programming.

There's a 0.05"-wide trace under sensor, which produces approximately 48 mV/A, or 170 mV at 3.5A Full Scale.

Connections: Sensor OUT+ to ADC2 (PB4); OUT- to ADC3 (PB3); calibrated PWM out on PB0; uncalibrated on PB1; Zeroing switch on PB2.
A voltage divider scales the 5V PWM  to read 35 mV (3.50A) on a three-digit meter.

Resources:
  • NVE Analog Sensor Datasheet: www.nve.com/Downloads/analog_catalog.pdf
  • NVE Current-Sensing Web Applications: www.nve.com/spec/calculators.php
  • Arduino IDE software: https://www.arduino.cc
  • Adafruit Trinket bootloader: https://github.com/adafruit/Adafruit-Trinket-Gemma-Bootloader
  • Trinket Drivers: https://learn.adafruit.com/usbtinyisp/drivers
  • Demonstration video : www.nve.com/Videos.php ; www.YouTube.com/NveCorporation
