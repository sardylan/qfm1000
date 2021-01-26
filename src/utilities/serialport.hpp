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


#ifndef __QFM1000__UTILITIES__SERIALPORT__H
#define __QFM1000__UTILITIES__SERIALPORT__H

#include <QtCore/QString>

#include <QtSerialPort/QSerialPort>

namespace qfm1000::utilities {

    class SerialPort {

    public:

        SerialPort();

        ~SerialPort();

        static QString prettyConfig(
                const QString &portName,
                const QSerialPort::BaudRate &baudRate,
                const QSerialPort::DataBits &dataBits,
                const QSerialPort::Parity &parity,
                const QSerialPort::StopBits &stopBits,
                const QSerialPort::FlowControl &flowControl
        );

    };

}

#endif
