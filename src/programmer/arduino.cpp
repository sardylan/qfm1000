/*
 * qFM1000
 * Copyright (C) 2020  Luca Cireddu (sardylan@gmail.com)
 * https://www.lucacireddu.it/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
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

#include <QtCore/QDebug>
#include <QtCore/QThread>

#include "arduino.hpp"

#define ARDUINO_PROGRAMMER_SERIAL_WAIT 30000
#define ARDUINO_PROGRAMMER_SERIAL_SLEEP 25

using namespace qfm1000::programmer;

ArduinoProgrammer::ArduinoProgrammer(QObject *parent) {
    serialPort = new QSerialPort(this);

#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
    connect(serialPort, &QSerialPort::errorOccurred, this, &ArduinoProgrammer::errorOccurred);
#endif

    ready = false;
}

ArduinoProgrammer::~ArduinoProgrammer() = default;

void ArduinoProgrammer::init(const QString &portName, QSerialPort::BaudRate portSpeed) {
    if (serialPort->isOpen())
        return;

    serialPort->setPortName(portName);
    serialPort->setBaudRate(portSpeed);

    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    serialPort->setFlowControl(QSerialPort::NoFlowControl);

    serialPort->open(QIODevice::ReadWrite);
    serialPort->clear();

    while (serialPort->bytesAvailable() < 1)
        serialPort->waitForReadyRead(ARDUINO_PROGRAMMER_SERIAL_WAIT);

    QByteArray data = serialPort->read(1);

    if (data.at(0) == ARDUINO_PROGRAMMER_PROTOCOL_READY) {
        ready = true;
        serialPort->clear();

#if QT_VERSION > QT_VERSION_CHECK(5, 12, 0)
        QMetaObject::invokeMethod(this, &ArduinoProgrammer::connected, Qt::QueuedConnection);
#else
        QMetaObject::invokeMethod(this, "connected", Qt::QueuedConnection);
#endif
    }
}

void ArduinoProgrammer::close() {
    if (serialPort->isOpen()) {
        serialPort->clear();
        serialPort->close();

#if QT_VERSION > QT_VERSION_CHECK(5, 12, 0)
        QMetaObject::invokeMethod(this, &ArduinoProgrammer::disconnected, Qt::QueuedConnection);
#else
        QMetaObject::invokeMethod(this, "disconnected", Qt::QueuedConnection);
#endif
    }

    ready = false;
}

bool ArduinoProgrammer::isReady() const {
    return serialPort->isOpen() && ready;
}

void ArduinoProgrammer::errorOccurred(QSerialPort::SerialPortError serialPortError) {
    if (serialPortError == QSerialPort::NoError)
        return;

    qDebug() << "ERROR:" << serialPortError;

#if QT_VERSION > QT_VERSION_CHECK(5, 12, 0)
    QMetaObject::invokeMethod(this, &ArduinoProgrammer::error, Qt::QueuedConnection);
#else
    QMetaObject::invokeMethod(this, "error", Qt::QueuedConnection);
#endif

    close();
}

void ArduinoProgrammer::read() {
    if (!isReady())
        return;

    QMetaObject::invokeMethod(this, "readEeprom", Qt::QueuedConnection);
}

void ArduinoProgrammer::write(const QByteArray &data) {
    if (!isReady())
        return;

    if (data.length() != ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT)
        return;

    QMetaObject::invokeMethod(this, "writeEeprom", Qt::QueuedConnection, Q_ARG(const QByteArray&, data));
}

void ArduinoProgrammer::readEeprom() {
    int i = 0;

    while (isReady() && i < ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT) {
        readPage(static_cast<uint8_t>(i));

        QThread::msleep(ARDUINO_PROGRAMMER_SERIAL_SLEEP);

        i++;
    }

    if (i == ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT) {
        QMetaObject::invokeMethod(this, "eepromRead", Qt::QueuedConnection, Q_ARG(QByteArray, eepromData));

#if QT_VERSION > QT_VERSION_CHECK(5, 12, 0)
        QMetaObject::invokeMethod(this, &ArduinoProgrammer::readCompleted, Qt::QueuedConnection);
#else
        QMetaObject::invokeMethod(this, "readCompleted", Qt::QueuedConnection);
#endif
    }
}

void ArduinoProgrammer::writeEeprom(const QByteArray &data) {
    int i = 0;

    while (isReady() && i < ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT) {
        QByteArray pageData;

        for (int j = 0; j < ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE; j++)
            pageData.append(data[(ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * i) + j]);

        writePage(static_cast<quint8>(i), pageData);

        QThread::msleep(ARDUINO_PROGRAMMER_SERIAL_SLEEP);

        i++;
    }

    if (i == ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT)
#if QT_VERSION > QT_VERSION_CHECK(5, 12, 0)
        QMetaObject::invokeMethod(this, &ArduinoProgrammer::writeCompleted, Qt::QueuedConnection);
#else
    QMetaObject::invokeMethod(this, "writeCompleted", Qt::QueuedConnection);
#endif
}

void ArduinoProgrammer::readPage(quint8 num) {
    QByteArray cmd;
    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_READ);
    cmd.append(num);

    serialPort->write(cmd);
    serialPort->waitForBytesWritten(ARDUINO_PROGRAMMER_SERIAL_WAIT);

    while (serialPort->bytesAvailable() < ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE)
        serialPort->waitForReadyRead(ARDUINO_PROGRAMMER_SERIAL_WAIT);

    QByteArray pageData = serialPort->read(8);
    eepromData.replace(ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE * num, ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE, pageData);

    qDebug()
            << "Page" << num << "read"
            << "-"
            << QByteArray(pageData).toHex();

    QMetaObject::invokeMethod(this, "pageRead", Qt::QueuedConnection, Q_ARG(quint8, num));
}

void ArduinoProgrammer::writePage(quint8 num, const QByteArray &data) {
    QByteArray cmd;

    cmd.append(ARDUINO_PROGRAMMER_PROTOCOL_WRITE);
    cmd.append(num);
    cmd.append(data);
    serialPort->write(cmd);
    serialPort->waitForBytesWritten(ARDUINO_PROGRAMMER_SERIAL_WAIT);

    while (serialPort->bytesAvailable() == 0)
        serialPort->waitForReadyRead(ARDUINO_PROGRAMMER_SERIAL_WAIT);

    QByteArray response = serialPort->read(1);

    qDebug()
            << "Page" << num << "written"
            << " - "
            << QByteArray(cmd).toHex()
            << " - "
            << response;

    if (response.at(0) == ARDUINO_PROGRAMMER_PROTOCOL_OK)
        QMetaObject::invokeMethod(this, "pageWritten", Qt::QueuedConnection, Q_ARG(quint8, num));
}
