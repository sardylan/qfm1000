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

#ifndef __QFM1000_CONFIG_CONFIG_H
#define __QFM1000_CONFIG_CONFIG_H

#include <QtCore/QString>
#include <QtSerialPort/QSerialPort>

class Config {

private:
    Config();

    static Config *instance;

    QString portName;
    QSerialPort::BaudRate portSpeed;

public:
    static Config *getInstance();

    const QString &getPortName() const;

    void setPortName(const QString &portName);

    QSerialPort::BaudRate getPortSpeed() const;

    void setPortSpeed(QSerialPort::BaudRate portSpeed);

};

#endif
