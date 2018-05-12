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

#ifndef __QFM1000_EEPROM_PROGRAMMER_H
#define __QFM1000_EEPROM_PROGRAMMER_H

#include <QtCore/QObject>
#include <QtSerialPort/QSerialPort>

#include <config.hpp>
#include <status.hpp>

#define ARDUINO_PROGRAMMER_EEPROM_PAGE_SIZE 8
#define ARDUINO_PROGRAMMER_EEPROM_PAGE_COUNT 256

#define ARDUINO_PROGRAMMER_PROTOCOL_READY 'L'
#define ARDUINO_PROGRAMMER_PROTOCOL_OK 'K'
#define ARDUINO_PROGRAMMER_PROTOCOL_ERROR 'E'
#define ARDUINO_PROGRAMMER_PROTOCOL_READ 'R'
#define ARDUINO_PROGRAMMER_PROTOCOL_WRITE 'W'

class ArduinoProgrammer : public QObject {
Q_OBJECT

public:
    explicit ArduinoProgrammer(QObject *parent = nullptr);

    ~ArduinoProgrammer() override;

    Q_INVOKABLE void init(QString portName, QSerialPort::BaudRate portSpeed);

    Q_INVOKABLE void close();

    bool isReady() const;

public slots:

    void read();

    void write(QByteArray data);

private:
    QSerialPort *serialPort;
    bool ready;
    QByteArray eepromData;

    Q_INVOKABLE void readEeprom();

    Q_INVOKABLE void writeEeprom(QByteArray data);

    void readPage(uint8_t num);

    void writePage(uint8_t num, QByteArray data);

private slots:

    void errorOccurred(QSerialPort::SerialPortError serialPortError);

signals:

    void connected();

    void disconnected();

    void error();

    void pageRead(uint8_t num);

    void pageWritten(uint8_t num);

    void readCompleted();

    void writeCompleted();

    void eepromRead(QByteArray data);
};

#endif
