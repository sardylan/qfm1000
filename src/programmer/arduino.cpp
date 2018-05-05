/*
 * qFM1000
 * Copyright (C) 2017  Luca Cireddu
 * sardylan@gmail.com
 * http://www.lucacireddu.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <QDebug>
#include <QFutureWatcher>
#include <QtConcurrent>

#include "arduino.hpp"

ArduinoProgrammer::ArduinoProgrammer(QObject *parent) {
    reset();
}

ArduinoProgrammer::~ArduinoProgrammer() = default;

void ArduinoProgrammer::init(const QString &portName, QSerialPort::BaudRate baudRate) {
    serialPort.setPortName(portName);
    serialPort.setBaudRate(baudRate);

    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);

    connect(&serialPort, SIGNAL(readyRead()), this, SLOT(readReadyResponse()));
    serialPort.open(QIODevice::ReadWrite);
}

void ArduinoProgrammer::close() {
    serialPort.close();
    reset();
}

void ArduinoProgrammer::read() {
    if (!ready)
        return;

    QtConcurrent::run(this, &ArduinoProgrammer::readEeprom);
}

void ArduinoProgrammer::write(QByteArray data) {
    if (!ready)
        return;

    if (data.length() != ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT)
        return;

    QtConcurrent::run(this, &ArduinoProgrammer::writeEeprom, data);
}

void ArduinoProgrammer::readEeprom() {

    for (uint8_t i = 0; i <= ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT - 1; i++)
        readPage(i);

    emit eepromRead(QByteArray(eepromData));
    emit readCompleted();
}

void ArduinoProgrammer::writeEeprom(QByteArray data) {
    for (uint8_t i = 0; i <= ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT - 1; i++) {
        QByteArray pageData;

        for (int j = 0; j < ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE; j++)
            pageData.append(data[(ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * i) + j]);

        writePage(i, pageData);
    }

    emit writeCompleted();
}

void ArduinoProgrammer::readPage(uint8_t num) {
    QByteArray cmd;
    char buff[ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE];

    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_READ);
    cmd.append((char) num);
    serialPort.write(cmd);

    serialPort.read(&buff[0], ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE);
    memcpy(&eepromData[ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * num], &buff[0], ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE);

    emit pageRead(num);
}

void ArduinoProgrammer::writePage(uint8_t num, QByteArray data) {
    QByteArray cmd;
    char c;

    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_WRITE);
    cmd.append((char) num);
    cmd.append(data);
    serialPort.write(cmd);

    serialPort.read(&c, 1);

    if (c == ARDUINO_PROGRAMMER_PROTOCOL_OK)
            emit pageWritten(num);
}

void ArduinoProgrammer::reset() {
    ready = false;
    memset(&eepromData[0], '\0', sizeof(eepromData));
}

void ArduinoProgrammer::readReadyResponse() {
    char c;

    while (serialPort.bytesAvailable() > 0) {
        if (serialPort.read(&c, 1) != 1)
            break;

        if (c == ARDUINO_PROGRAMMER_PROTOCOL_READY) {
            ready = true;
            disconnect(&serialPort, SIGNAL(readyRead()), this, SLOT(readReadyResponse()));
        }
    }
}
