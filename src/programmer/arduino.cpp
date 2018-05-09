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
    status = Status::getInstance();
    config = Config::getInstance();

    reset();
}

ArduinoProgrammer::~ArduinoProgrammer() = default;

void ArduinoProgrammer::init() {
    if (serialPort.isOpen())
        return;

    serialPort.setPortName(config->getArduinoPortName());
    serialPort.setBaudRate(config->getArduinoPortSpeed());

    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);

    serialPort.open(QIODevice::ReadWrite);
    serialPort.clear();
    connect(&serialPort, &QSerialPort::readyRead, this, &ArduinoProgrammer::readReadyResponse);
    connect(&serialPort, &QSerialPort::errorOccurred, this, &ArduinoProgrammer::errorOccurred);
}

void ArduinoProgrammer::close() {
    if (serialPort.isOpen()) {
        serialPort.clear();
        serialPort.close();
    }

    reset();
    emit disconnected();
}

bool ArduinoProgrammer::isReady() const {
    return serialPort.isOpen() && ready;
}

void ArduinoProgrammer::read() {
    if (!isReady())
        return;

//    QtConcurrent::run(this, &ArduinoProgrammer::readEeprom);
    readEeprom();
}

void ArduinoProgrammer::write(QByteArray data) {
    if (!isReady())
        return;

    if (data.length() != ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT)
        return;

//    QtConcurrent::run(this, &ArduinoProgrammer::writeEeprom, data);
    writeEeprom(data);
}

void ArduinoProgrammer::readEeprom() {
    for (int i = 0; i <= ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT - 1; i++)
        readPage(static_cast<uint8_t>(i));

    emit eepromRead(QByteArray(eepromData));
    emit readCompleted();
}

void ArduinoProgrammer::writeEeprom(QByteArray data) {
    for (int i = 0; i <= ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT - 1; i++) {
        QByteArray pageData;

        for (int j = 0; j < ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE; j++)
            pageData.append(data[(ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * i) + j]);

        writePage(static_cast<uint8_t>(i), pageData);
    }

    emit writeCompleted();
}

void ArduinoProgrammer::readPage(uint8_t num) {
    QByteArray cmd;
    char buff[ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE];

    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_READ);
    cmd.append(num);
    serialPort.write(cmd);
    serialPort.waitForBytesWritten();

    serialPort.read(&buff[0], ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE);
    memcpy(&eepromData[ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * num], &buff[0], ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE);

    qDebug() << "Read page" << num << " - " << QByteArray(cmd).toHex() << " - " << QByteArray(buff).toHex();
    emit pageRead(num);
}

void ArduinoProgrammer::writePage(uint8_t num, QByteArray data) {
    QByteArray cmd;
    char c;

    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_WRITE);
    cmd.append(num);
    cmd.append(data);
    serialPort.write(cmd);
    serialPort.waitForBytesWritten();

    serialPort.waitForReadyRead();
    serialPort.read(&c, 1);

    qDebug() << "Write page" << num << " - " << QByteArray(cmd).toHex() << " - " << c;
    if (c == ARDUINO_PROGRAMMER_PROTOCOL_OK)
            emit pageWritten(num);
}

void ArduinoProgrammer::reset() {
    ready = false;
    memset(&eepromData[0], '\0', sizeof(eepromData));

    status->setSerialEepromOpened(false);
}

void ArduinoProgrammer::readReadyResponse() {
    char c;

    while (serialPort.bytesAvailable() > 0) {
        if (serialPort.read(&c, 1) != 1)
            break;

        if (c == ARDUINO_PROGRAMMER_PROTOCOL_READY) {
            disconnect(&serialPort, SIGNAL(readyRead()), this, SLOT(readReadyResponse()));
            serialPort.clear();

            ready = true;

            status->setSerialEepromOpened(true);
            emit connected();
        }
    }
}

void ArduinoProgrammer::errorOccurred(QSerialPort::SerialPortError serialPortError) {
    emit error();

//    serialPort.clearError();
    if (serialPort.isOpen())
        serialPort.close();

    reset();
    emit disconnected();
}