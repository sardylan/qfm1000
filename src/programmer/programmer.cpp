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

#include "programmer.hpp"

ArduinoProgrammer::ArduinoProgrammer(QObject *parent) {

}

ArduinoProgrammer::~ArduinoProgrammer() {
}

void ArduinoProgrammer::init(const QString &portName, QSerialPort::BaudRate baudRate) {
    serialPort.setPortName(portName);
    serialPort.setBaudRate(baudRate);

    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);

    serialPort.open(QIODevice::ReadWrite);
}

void ArduinoProgrammer::close() {
    serialPort.close();
}

QByteArray ArduinoProgrammer::read() {
    if (!serialPort.isOpen())
        return QByteArray();

    QByteArray data;

    data.clear();

    for (uint8_t i = 0; i <= 0xff; i++)
        data.append(readPage(i));

    return data;
}

void ArduinoProgrammer::write(const QByteArray &data) {
    if (!serialPort.isOpen())
        return;

    if (data.length() != ARDUINO_PROGRAMMER_BUFFER_SIZE * 256)
        return;

    for (uint8_t i = 0; i <= 0xff; i++)
        writePage(i, data.mid(i * ARDUINO_PROGRAMMER_BUFFER_SIZE, ARDUINO_PROGRAMMER_BUFFER_SIZE));
}

QByteArray ArduinoProgrammer::readPage(uint8_t page) {
    QByteArray cmd;
    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_READ);
    cmd.append((char) page);
    serialPort.write(cmd);

    while (serialPort.bytesAvailable() < ARDUINO_PROGRAMMER_BUFFER_SIZE)
        serialPort.waitForReadyRead(100);

    QByteArray data = serialPort.readAll();

    if (data.length() != ARDUINO_PROGRAMMER_BUFFER_SIZE)
        return QByteArray();

    return data;
}

void ArduinoProgrammer::writePage(uint8_t page, const QByteArray &data) {
    if (data.length() != ARDUINO_PROGRAMMER_BUFFER_SIZE)
        return;

    QByteArray cmd;
    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_WRITE);
    cmd.append((char) page);
    cmd.append(data);
    serialPort.write(cmd);
}
