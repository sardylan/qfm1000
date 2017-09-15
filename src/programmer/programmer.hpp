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

#define ARDUINO_PROGRAMMER_BUFFER_SIZE 8

#define ARDUINO_PROGRAMMER_PROTOCOL_OK 'K'
#define ARDUINO_PROGRAMMER_PROTOCOL_ERROR 'E'
#define ARDUINO_PROGRAMMER_PROTOCOL_READ 'R'
#define ARDUINO_PROGRAMMER_PROTOCOL_WRITE 'W'

class ArduinoProgrammer : public QObject {
Q_OBJECT

public:
    explicit ArduinoProgrammer(QObject *parent = nullptr);

    ~ArduinoProgrammer();

    void init(const QString &portName, QSerialPort::BaudRate baudRate);

    void close();

    QByteArray read();

    void write(const QByteArray &data);

private:
    QSerialPort serialPort;

    QByteArray readPage(uint8_t page);

    void writePage(uint8_t page, const QByteArray &data);

};

#endif
