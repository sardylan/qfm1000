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
#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent>

#include "programmer.hpp"

ArduinoProgrammer::ArduinoProgrammer(QObject *parent) {
    ready = false;
}

ArduinoProgrammer::~ArduinoProgrammer() = default;

void ArduinoProgrammer::init(const QString &portName, QSerialPort::BaudRate baudRate) {
    serialPort.setPortName(portName);
    serialPort.setBaudRate(baudRate);

    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);

    connect(&serialPort, SIGNAL(readyRead()), this, SLOT(catchReadyCommand()));
    serialPort.open(QIODevice::ReadWrite);
    serialPort.waitForReadyRead(3000);
}

void ArduinoProgrammer::close() {
    ready = false;
    serialPort.close();
}

void ArduinoProgrammer::read() {
    if (!ready || !serialPort.isOpen())
        return;

    QByteArray data;

    data.clear();

    QFuture<void> future = QtConcurrent::run(this, &ArduinoProgrammer::readPages);

    QFutureWatcher<void> futureWatcher;
    futureWatcher.setFuture(future);
    QObject::connect(&futureWatcher, SIGNAL(finished()), this, SLOT(eepromReadFinish()));
}

void ArduinoProgrammer::eepromReadFinish() {
    emit eepromRead(data);
}

void ArduinoProgrammer::readPages() {
    for (int i = 0; i <= 255; i++)
        data.append(readPage(static_cast<uint8_t>(i)));
}

QByteArray ArduinoProgrammer::readPage(uint8_t page) {
    QByteArray cmd;
    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_READ);
    cmd.append((char) page);
    serialPort.write(cmd);
    serialPort.waitForBytesWritten(1000);

    while (serialPort.bytesAvailable() < ARDUINO_PROGRAMMER_BUFFER_SIZE)
        serialPort.waitForReadyRead(100);

    QByteArray data = serialPort.readAll();

    if (data.length() != ARDUINO_PROGRAMMER_BUFFER_SIZE)
        return QByteArray();

    emit pageRead(page);

    return data;
}

void ArduinoProgrammer::write(const QByteArray &data) {
    if (!ready || !serialPort.isOpen())
        return;

    if (data.length() != ARDUINO_PROGRAMMER_BUFFER_SIZE * 256)
        return;

    this->data = data;

    QFuture<void> future = QtConcurrent::run(this, &ArduinoProgrammer::writePages);

    QFutureWatcher<void> futureWatcher;
    futureWatcher.setFuture(future);
    QObject::connect(&futureWatcher, SIGNAL(finished()), this, SLOT(eepromWriteFinish()));
}

void ArduinoProgrammer::eepromWriteFinish() {
    emit eepromWritten();
}

void ArduinoProgrammer::writePages() {
    for (int i = 0; i <= 255; i++)
        writePage(static_cast<uint8_t>(i),
                  data.mid(i * ARDUINO_PROGRAMMER_BUFFER_SIZE, ARDUINO_PROGRAMMER_BUFFER_SIZE));
}

void ArduinoProgrammer::writePage(uint8_t page, const QByteArray &data) {
    if (data.length() != ARDUINO_PROGRAMMER_BUFFER_SIZE)
        return;

    QByteArray cmd;
    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_WRITE);
    cmd.append((char) page);
    cmd.append(data);
    serialPort.write(cmd);
    serialPort.waitForBytesWritten(1000);

    serialPort.waitForReadyRead(1000);
    QByteArray response = serialPort.readAll();
    if (response.length() != 1)
        return;

    bool result = false;
    if (response.at(0) == ARDUINO_PROGRAMMER_PROTOCOL_OK)
        result = true;

    emit pageWritten(page, result);
}

void ArduinoProgrammer::catchReadyCommand() {
    QByteArray data = serialPort.readAll();

    if (data.length() == 1 && data.at(0) == ARDUINO_PROGRAMMER_PROTOCOL_READY) {
        ready = true;
        disconnect(&serialPort, SIGNAL(readyRead()), this, SLOT(catchReadyCommand()));
    }
}

bool ArduinoProgrammer::isReady() const {
    return ready;
}
