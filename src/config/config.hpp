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

#ifndef __QFM1000_CONFIG_CONFIG_H
#define __QFM1000_CONFIG_CONFIG_H

#include <QtCore/QString>
#include <QtSerialPort/QSerialPort>

class Config {

private:
    Config();

    static Config *instance;

    QString arduinoPortName;
    QSerialPort::BaudRate arduinoPortSpeed;
    QString radioPortName;
    QSerialPort::BaudRate radioPortSpeed;

public:
    static Config *getInstance();

    const QString &getArduinoPortName() const;

    void setArduinoPortName(const QString &arduinoPortName);

    QSerialPort::BaudRate getArduinoPortSpeed() const;

    void setArduinoPortSpeed(QSerialPort::BaudRate arduinoPortSpeed);

    const QString &getRadioPortName() const;

    void setRadioPortName(const QString &radioPortName);

    QSerialPort::BaudRate getRadioPortSpeed() const;

    void setRadioPortSpeed(QSerialPort::BaudRate radioPortSpeed);
};

#endif
