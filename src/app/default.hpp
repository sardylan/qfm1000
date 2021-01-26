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


#ifndef __QFM1000__QFM1000__DEFAULT__H
#define __QFM1000__QFM1000__DEFAULT__H

#define CONFIG_SERIALPORT_PORTNAME_TAG "portName"
#define CONFIG_SERIALPORT_BAUDRATE_TAG "baudRate"
#define CONFIG_SERIALPORT_DATABITS_TAG "dataBits"
#define CONFIG_SERIALPORT_PARITY_TAG "parity"
#define CONFIG_SERIALPORT_STOPBITS_TAG "stopBits"
#define CONFIG_SERIALPORT_FLOWCONTROL_TAG "flowControl"

#define CONFIG_INOPROG_TAG "InoProg"

#ifdef Q_OS_LINUX
#define CONFIG_INOPROG_PORTNAME_DEFAULT "/dev/ttyACM0"
#endif

#ifdef Q_OS_WINDOWS
#define CONFIG_INOPROG_PORTNAME_DEFAULT "COM3"
#endif

#ifndef CONFIG_INOPROG_PORTNAME_DEFAULT
#define CONFIG_INOPROG_PORTNAME_DEFAULT ""
#endif

#define CONFIG_INOPROG_BAUDRATE_DEFAULT QSerialPort::Baud115200
#define CONFIG_INOPROG_DATABITS_DEFAULT QSerialPort::Data8
#define CONFIG_INOPROG_PARITY_DEFAULT QSerialPort::NoParity
#define CONFIG_INOPROG_STOPBITS_DEFAULT QSerialPort::OneStop
#define CONFIG_INOPROG_FLOWCONTROL_DEFAULT QSerialPort::NoFlowControl

#endif
