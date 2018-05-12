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

#define ARDUINO_PROGRAMMER_SERIAL_SLEEP 25

ArduinoProgrammer::ArduinoProgrammer(QObject *parent) {
    status = Status::getInstance();
    config = Config::getInstance();

    serialPort = new QSerialPort();

    reset();
}

ArduinoProgrammer::~ArduinoProgrammer() {
    delete serialPort;
}

void ArduinoProgrammer::init() {
    if (serialPort->isOpen())
        return;

    serialPort->setPortName(config->getArduinoPortName());
    serialPort->setBaudRate(config->getArduinoPortSpeed());

    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    serialPort->open(QIODevice::ReadWrite);
    serialPort->clear();

    connect(serialPort, &QSerialPort::readyRead, this, &ArduinoProgrammer::readReadyResponse);
    connect(serialPort, &QSerialPort::errorOccurred, this, &ArduinoProgrammer::errorOccurred);
}

void ArduinoProgrammer::close() {
    if (serialPort->isOpen()) {
        serialPort->clear();
        serialPort->close();
    }

    reset();
    emit disconnected();
}

bool ArduinoProgrammer::isReady() const {
    return serialPort->isOpen() && ready;
}

void ArduinoProgrammer::read() {
    if (!isReady())
        return;

    QMetaObject::invokeMethod(this, "readEeprom", Qt::QueuedConnection);
}

void ArduinoProgrammer::write(QByteArray data) {
    if (!isReady())
        return;

    if (data.length() != ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT)
        return;

    QMetaObject::invokeMethod(this, "writeEeprom", Qt::QueuedConnection, Q_ARG(QByteArray, data));
}

void ArduinoProgrammer::readEeprom() {
    for (int i = 0; i <= ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT - 1; i++) {
        readPage(static_cast<uint8_t>(i));
        QThread::msleep(ARDUINO_PROGRAMMER_SERIAL_SLEEP);
    }

    emit eepromRead(QByteArray(eepromData));
    emit readCompleted();
}

void ArduinoProgrammer::writeEeprom(QByteArray data) {
    for (int i = 0; i <= ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT - 1; i++) {
        QByteArray pageData;

        for (int j = 0; j < ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE; j++)
            pageData.append(data[(ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * i) + j]);

        writePage(static_cast<uint8_t>(i), pageData);
        QThread::msleep(ARDUINO_PROGRAMMER_SERIAL_SLEEP);
    }

    emit writeCompleted();
}

void ArduinoProgrammer::readPage(uint8_t num) {
    QByteArray cmd;
    QByteArray buff;

    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_READ);
    cmd.append(num);
    serialPort->write(cmd);
    serialPort->waitForBytesWritten();

    buff.clear();
    while (buff.size() != ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE && serialPort->waitForReadyRead(1000)) {
        buff.append(serialPort->read(1));
        qDebug() << "Buff:" << buff.toHex();
    }

    qDebug() << "Read page" << num << " - " << QByteArray(cmd).toHex() << " - " << QByteArray(buff).toHex();

    if (buff.length() == ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE) {
        eepromData.replace(ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * num, ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE, buff);
        emit pageRead(num);
    }
}

void ArduinoProgrammer::writePage(uint8_t num, QByteArray data) {
    QByteArray cmd;
    QByteArray response;

    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_WRITE);
    cmd.append(num);
    cmd.append(data);
    serialPort->write(cmd);
    serialPort->waitForBytesWritten();

    serialPort->waitForReadyRead();
    response = serialPort->readAll();

    qDebug() << "Write page" << num << " - " << QByteArray(cmd).toHex() << " - " << response;
    if (response.length() > 0 && response.at(0) == ARDUINO_PROGRAMMER_PROTOCOL_OK)
            emit pageWritten(num);
}

void ArduinoProgrammer::reset() {
    ready = false;
    eepromData.clear();
    eepromData.append(ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT * ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE, '\0');

    status->setSerialEepromOpened(false);
}

void ArduinoProgrammer::readReadyResponse() {
    char c;

    while (serialPort->bytesAvailable() > 0) {
        if (serialPort->read(&c, 1) != 1)
            break;

        if (c == ARDUINO_PROGRAMMER_PROTOCOL_READY) {
            disconnect(serialPort, &QSerialPort::readyRead, this, &ArduinoProgrammer::readReadyResponse);
            serialPort->clear();

            ready = true;

            status->setSerialEepromOpened(true);
            emit connected();
        }
    }
}

void ArduinoProgrammer::errorOccurred(QSerialPort::SerialPortError serialPortError) {
    qDebug() << "ERROR" << serialPortError;

    emit error();

//    serialPort->clearError();
    if (serialPort->isOpen())
        serialPort->close();

    reset();
    emit disconnected();
}