/*
 * qFM1000
 * Copyright (C) 2021  Luca Cireddu - IS0GVH
 * sardylan@gmail.com - is0gvh@gmail.com
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
#include <QtCore/QFuture>

#include <QtConcurrent/QtConcurrent>

#include "inoprog.hpp"

using namespace qfm1000;
using namespace qfm1000::inoprog;

#define INOPROG_SERIAL_WAIT 5000
#define INOPROG_SERIAL_SLEEP 25

InoProg::InoProg(QObject *parent) : Service(parent) {
    portSpeed = QSerialPort::Baud115200;
    ready = false;

    connect(&serialPort, &QSerialPort::errorOccurred, this, &InoProg::errorOccurred);
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

void InoProg::connectSignals() {
    connect(&serialPort, &QSerialPort::errorOccurred, this, &InoProg::errorOccurred);
}

bool InoProg::isReady() {
    return serialPort.isOpen() && ready;
}

void InoProg::start() {
    mutex.lock();

    if (!serialPort.isOpen())
        programmerStart();

    mutex.unlock();
}

void InoProg::stop() {
    mutex.lock();

    if (serialPort.isOpen())
        programmerStop();

    mutex.unlock();
}

void InoProg::programmerStart() {
    serialPort.setPortName(portName);
    serialPort.setBaudRate(portSpeed);

    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);

    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    if (!serialPort.open(QIODevice::ReadWrite)) {
        qCritical() << "Unable to open serial port";
        emitError(InoProgError::INOPROG_ERROR_UNABLE_OPEN_SERIAL);
        programmerStop(false);
        return;
    }

    serialPort.clear();

    if (serialPort.bytesAvailable() < 1)
        serialPort.waitForReadyRead(INOPROG_SERIAL_WAIT);

    if (serialPort.bytesAvailable() < 1) {
        qCritical() << "Arduino not answering";
        emitError(InoProgError::INOPROG_ERROR_NO_ANSWER);
        programmerStop(false);
        return;
    }

    QByteArray data = serialPort.read(1);
    if (data.at(0) != INOPROG_PROTOCOL_READY) {
        qCritical() << "Arduino not ready";
        emitError(InoProgError::INOPROG_ERROR_NOT_READY);
        programmerStop(false);
        return;
    }

    ready = true;
    serialPort.clear();
    QMetaObject::invokeMethod(this, &InoProg::connected, Qt::QueuedConnection);
}

void InoProg::programmerStop(bool sendSignal) {
    serialPort.clear();
    serialPort.close();

    ready = false;

    if (sendSignal)
        QMetaObject::invokeMethod(this, &InoProg::disconnected, Qt::QueuedConnection);
}

void InoProg::errorOccurred(QSerialPort::SerialPortError serialPortError) {
    if (serialPortError == QSerialPort::NoError)
        return;

    qCritical() << "ERROR:" << serialPortError;
    emitError(InoProgError::INOPROG_ERROR_SERIAL_PORT);

    stop();
}

QByteArray InoProg::readEeprom() {
    QByteArray eepromData;

    mutex.lock();

    emitProgress(-1);

    if (isReady()) {
        eepromData.clear();

        for (int p = 0; p < INOPROG_EEPROM_PAGE_COUNT; p++) {
            auto pageNum = (PageNum) p;

            QFuture<QByteArray> readPageFuture = QtConcurrent::run(&InoProg::readPage, this, pageNum);
            QByteArray pageData = readPageFuture.result();

            if (pageData.size() != INOPROG_EEPROM_PAGE_SIZE) {
                qCritical() << "Error reading page" << pageNum;
                emitError(InoProgError::INOPROG_ERROR_PAGE);
                eepromData.clear();
                break;
            }

            eepromData.append(pageData);

            emitProgress(p);

            QThread::msleep(INOPROG_SERIAL_SLEEP);
        }

        if (eepromData.size() == INOPROG_EEPROM_PAGE_COUNT * INOPROG_EEPROM_PAGE_SIZE) {
            emitProgress(INOPROG_EEPROM_PAGE_COUNT);
            QMetaObject::invokeMethod(this, "readCompleted", Qt::QueuedConnection);
        }
    } else {
        qCritical() << "Serial port is not ready";
        emitError(InoProgError::INOPROG_ERROR_SERIAL_PORT);
        eepromData.clear();
    }

    mutex.unlock();

    return eepromData;
}

void InoProg::writeEeprom(const QByteArray &data) {
    mutex.lock();

    emitProgress(-1);

    if (isReady()) {
        bool errors = false;

        for (int p = 0; p < INOPROG_EEPROM_PAGE_COUNT; p++) {
            auto pageNum = (PageNum) p;

            QByteArray pageData = data.mid(INOPROG_EEPROM_PAGE_SIZE * pageNum, INOPROG_EEPROM_PAGE_SIZE);
            QFuture<bool> writePageFuture = QtConcurrent::run(&InoProg::writePage, this, pageNum, pageData);
            bool result = writePageFuture.result();

            if (!result) {
                qCritical() << "Error writing page" << pageNum;
                emitError(InoProgError::INOPROG_ERROR_PAGE);
                errors = true;
                break;
            }

            emitProgress(p);

            QThread::msleep(INOPROG_SERIAL_SLEEP);
        }

        if (!errors) {
            emitProgress(INOPROG_EEPROM_PAGE_COUNT);
            QMetaObject::invokeMethod(this, "writeCompleted", Qt::QueuedConnection);
        }
    } else {
        qCritical() << "Serial port is not ready";
        emitError(InoProgError::INOPROG_ERROR_SERIAL_PORT);
    }

    mutex.unlock();
}

QByteArray InoProg::readPage(PageNum num) {
    QByteArray cmd;
    cmd.append(INOPROG_PROTOCOL_READ);
    cmd.append(num);

    serialPort.write(cmd);
    serialPort.waitForBytesWritten(INOPROG_SERIAL_WAIT);

    if (serialPort.bytesAvailable() < INOPROG_EEPROM_PAGE_SIZE)
        serialPort.waitForReadyRead(INOPROG_SERIAL_WAIT);

    if (serialPort.bytesAvailable() < INOPROG_EEPROM_PAGE_SIZE)
        return QByteArray();

    QByteArray pageData = serialPort.read(INOPROG_EEPROM_PAGE_SIZE);

    QMetaObject::invokeMethod(this, "pageRead", Qt::QueuedConnection, Q_ARG(PageNum, num));
    qDebug() << "Read page" << num
             << " - "
             << pageData.toHex();

    return pageData;
}

bool InoProg::writePage(PageNum num, const QByteArray &pageData) {
    QByteArray cmd;

    cmd.append(INOPROG_PROTOCOL_WRITE);
    cmd.append(num);
    cmd.append(pageData);

    serialPort.write(cmd);
    serialPort.waitForBytesWritten(INOPROG_SERIAL_WAIT);

    if (serialPort.bytesAvailable() < 1)
        serialPort.waitForReadyRead(INOPROG_SERIAL_WAIT);

    if (serialPort.bytesAvailable() < 1)
        return false;

    QByteArray response = serialPort.read(1);

    if (response.at(0) == INOPROG_PROTOCOL_OK)
        QMetaObject::invokeMethod(this, "pageWritten", Qt::QueuedConnection, Q_ARG(PageNum, num));

    qDebug() << "Write page" << num
             << " - "
             << pageData.toHex()
             << " - "
             << "Response" << response;

    return true;
}

void InoProg::emitProgress(int value) {
    QMetaObject::invokeMethod(
            this,
            "progress",
            Qt::QueuedConnection,
            Q_ARG(int, INOPROG_EEPROM_PAGE_COUNT),
            Q_ARG(int, value)
    );
}

void InoProg::emitError(InoProgError inoProgError) {
    QMetaObject::invokeMethod(
            this,
            "error",
            Qt::QueuedConnection,
            Q_ARG(InoProgError, inoProgError)
    );
}

void InoProg::registerMetaTypes() {
    qRegisterMetaType<qfm1000::inoprog::InoProgError>("qfm1000::inoprog::InoProgError");
}
