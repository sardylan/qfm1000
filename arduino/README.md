# InoProg

This page is for InoProg, and Arduino Programmer for reading 24C16 EEPROMs.

Reading and Writing EEPROMs from qFM1000 is directly supported using dedicated buttons:

- ![Read](https://github.com/sardylan/qfm1000/raw/master/doc/img/eeprom-read.png) Read EEPROM
- ![Write](https://github.com/sardylan/qfm1000/raw/master/doc/img/eeprom-write.png) Write EEPROM

You will need an Arduino Uno (original or compatibile) with a custom sketch, available in this repository.
It uses i2c capabilities embedded on ATMega328P chips to communicate with 24C16 EEPROM and Arduino Serial Port to communicate with PC and qFM1000.

Connections schema is [available here](https://github.com/sardylan/qfm1000/blob/master/doc/pdf/InoProg.pdf)

![InoProg 1](https://github.com/sardylan/qfm1000/raw/master/doc/img/inoprog-01.jpg)
![InoProg 2](https://github.com/sardylan/qfm1000/raw/master/doc/img/inoprog-02.jpg)
