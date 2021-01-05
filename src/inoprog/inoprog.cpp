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

#include <QtCore/QDebug>
#include <QtCore/QThread>
#include <QtCore/QFuture>

#include <QtConcurrent/QtConcurrent>

#include "inoprog.hpp"

using namespace qfm1000;
using namespace qfm1000::inoprog;

#define INOPROG_SERIAL_WAIT 30000
#define INOPROG_SERIAL_SLEEP 25

InoProg::InoProg(QObject *parent) : Service(parent) {
#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
    connect(&serialPort, &QSerialPort::errorOccurred, this, &InoProg::errorOccurred);
#endif

    ready = false;
}

InoProg::~InoProg() = default;

const QString &InoProg::getPortName() const {
    return portName;
}

void InoProg::setPortName(const QString &newValue) {
    InoProg::portName = newValue;
}

QSerialPort::BaudRate InoProg::getPortSpeed() const {
    return portSpeed;
}

void InoProg::setPortSpeed(QSerialPort::BaudRate newValue) {
    InoProg::portSpeed = newValue;
}

bool InoProg::isReady() {
    return serialPort.isOpen() && ready;
}

void InoProg::start() {
    mutex.lock();

    if (!serialPort.isOpen()) {
        serialPort.setPortName(portName);
        serialPort.setBaudRate(portSpeed);

        serialPort.setDataBits(QSerialPort::Data8);
        serialPort.setParity(QSerialPort::NoParity);
        serialPort.setStopBits(QSerialPort::OneStop);

        serialPort.setFlowControl(QSerialPort::NoFlowControl);

        if (serialPort.open(QIODevice::ReadWrite)) {
            serialPort.clear();

            while (serialPort.bytesAvailable() < 1)
                serialPort.waitForReadyRead(INOPROG_SERIAL_WAIT);

            QByteArray data = serialPort.read(1);
            if (data.at(0) == INOPROG_PROTOCOL_READY) {
                ready = true;
                serialPort.clear();
                QMetaObject::invokeMethod(this, &InoProg::connected, Qt::QueuedConnection);
            }
        } else {
            qCritical() << "Unable to open serial port";
        }
    }

    mutex.unlock();
}

void InoProg::stop() {
    mutex.lock();

    if (serialPort.isOpen()) {
        serialPort.clear();
        serialPort.close();
        QMetaObject::invokeMethod(this, &InoProg::disconnected, Qt::QueuedConnection);
    }

    ready = false;

    mutex.unlock();
}

void InoProg::errorOccurred(QSerialPort::SerialPortError serialPortError) {
    if (serialPortError == QSerialPort::NoError)
        return;

    qDebug() << "ERROR:" << serialPortError;

    QMetaObject::invokeMethod(this, &InoProg::error, Qt::QueuedConnection);

    stop();
}

QByteArray InoProg::readEeprom() {
    QByteArray eepromData;

    mutex.lock();

    if (isReady()) {
        eepromData.clear();

        for (int p = 0; p < INOPROG_EEPROM_PAGE_COUNT; p++) {
            auto pageNum = (PageNum) p;

            QFuture<QByteArray> pageDataFuture = QtConcurrent::run(this, &InoProg::readPage, pageNum);
            QByteArray pageData = pageDataFuture.result();

            if (pageData.size() != INOPROG_EEPROM_PAGE_SIZE) {
                qCritical() << "Error reading page" << pageNum;
                eepromData.clear();
                break;
            }

            eepromData.append(pageData);

            QThread::msleep(INOPROG_SERIAL_SLEEP);
        }

        QMetaObject::invokeMethod(this, &InoProg::readCompleted, Qt::QueuedConnection);
    } else {
        qWarning() << "Serial port is not ready";
        eepromData.clear();
    }

    mutex.unlock();

    return eepromData;
}

void InoProg::writeEeprom(const QByteArray &data) {
    mutex.lock();

    if (isReady()) {
        for (int p = 0; p < INOPROG_EEPROM_PAGE_COUNT; p++) {
            auto pageNum = (PageNum) p;

            QByteArray pageData = data.mid(INOPROG_EEPROM_PAGE_SIZE * pageNum, INOPROG_EEPROM_PAGE_SIZE);
            QFuture<void> pageDataFuture = QtConcurrent::run(this, &InoProg::writePage, pageNum, pageData);
            pageDataFuture.waitForFinished();

            QThread::msleep(INOPROG_SERIAL_SLEEP);
        }

        QMetaObject::invokeMethod(this, &InoProg::writeCompleted, Qt::QueuedConnection);
    } else {
        qWarning() << "Serial port is not ready";
    }

    mutex.unlock();
}

QByteArray InoProg::readPage(PageNum num) {
    QByteArray cmd;
    cmd.append(INOPROG_PROTOCOL_READ);
    cmd.append(num);

    serialPort.write(cmd);
    serialPort.waitForBytesWritten(INOPROG_SERIAL_WAIT);

    while (serialPort.bytesAvailable() < INOPROG_EEPROM_PAGE_SIZE)
        serialPort.waitForReadyRead(INOPROG_SERIAL_WAIT);

    QByteArray pageData = serialPort.read(INOPROG_EEPROM_PAGE_SIZE);

    QMetaObject::invokeMethod(this, "pageRead", Qt::QueuedConnection, Q_ARG(PageNum, num));
    qDebug() << "Read page" << num
             << " - "
             << pageData.toHex();

    return pageData;
}

void InoProg::writePage(PageNum num, const QByteArray &pageData) {
    QByteArray cmd;

    cmd.append(INOPROG_PROTOCOL_WRITE);
    cmd.append(num);
    cmd.append(pageData);

    serialPort.write(cmd);
    serialPort.waitForBytesWritten(INOPROG_SERIAL_WAIT);

    while (serialPort.bytesAvailable() == 0)
        serialPort.waitForReadyRead(INOPROG_SERIAL_WAIT);

    QByteArray response = serialPort.read(1);

    if (response.at(0) == INOPROG_PROTOCOL_OK)
        QMetaObject::invokeMethod(this, "pageWritten", Qt::QueuedConnection, Q_ARG(PageNum, num));

    qDebug() << "Write page" << num
             << " - "
             << pageData.toHex()
             << " - "
             << "Response" << response;
}