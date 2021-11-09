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


#include "serialport.hpp"

using namespace qfm1000::utilities;

SerialPort::SerialPort() = default;

SerialPort::~SerialPort() = default;

QString SerialPort::prettyConfig(
        const QString &portName,
        const QSerialPort::BaudRate &baudRate,
        const QSerialPort::DataBits &dataBits,
        const QSerialPort::Parity &parity,
        const QSerialPort::StopBits &stopBits,
        const QSerialPort::FlowControl &flowControl
) {
    QString baudRateString;

    switch (baudRate) {

        case QSerialPort::Baud1200:
            baudRateString = "1200";
            break;

        case QSerialPort::Baud2400:
            baudRateString = "2400";
            break;

        case QSerialPort::Baud4800:
            baudRateString = "4800";
            break;

        case QSerialPort::Baud9600:
            baudRateString = "9600";
            break;

        case QSerialPort::Baud19200:
            baudRateString = "19200";
            break;

        case QSerialPort::Baud38400:
            baudRateString = "38400";
            break;

        case QSerialPort::Baud57600:
            baudRateString = "57600";
            break;

        case QSerialPort::Baud115200:
            baudRateString = "115200";
            break;

        default:
            baudRateString = "UNKNOWN";
            break;
    }

    QString dataBitsString;

    switch (dataBits) {
        case QSerialPort::Data5:
            dataBitsString = "5";
            break;

        case QSerialPort::Data6:
            dataBitsString = "6";
            break;

        case QSerialPort::Data7:
            dataBitsString = "7";
            break;

        case QSerialPort::Data8:
            dataBitsString = "8";
            break;

        default:
            dataBitsString = "?";
            break;
    }

    QString parityString;

    switch (parity) {
        case QSerialPort::NoParity:
            parityString = "N";
            break;

        case QSerialPort::EvenParity:
            parityString = "E";
            break;

        case QSerialPort::OddParity:
            parityString = "O";
            break;

        case QSerialPort::SpaceParity:
            parityString = "S";
            break;

        case QSerialPort::MarkParity:
            parityString = "M";
            break;

        default:
            parityString = "?";
            break;
    }

    QString stopBitsString;

    switch (stopBits) {
        case QSerialPort::OneStop:
            stopBitsString = "1";
            break;

        case QSerialPort::OneAndHalfStop:
            stopBitsString = "1.5";
            break;

        case QSerialPort::TwoStop:
            stopBitsString = "2";
            break;

        default:
            stopBitsString = "?";
            break;
    }

    QString flowControlString;

    switch (flowControl) {
        case QSerialPort::NoFlowControl:
            flowControlString = "NO";
            break;

        case QSerialPort::HardwareControl:
            flowControlString = "HW";
            break;

        case QSerialPort::SoftwareControl:
            flowControlString = "SW";
            break;

        default:
            flowControlString = "??";
            break;
    }

    return QString("%1 %2 %3-%4-%5 %6")
            .arg(portName, baudRateString, dataBitsString, parityString, stopBitsString, flowControlString);
}
