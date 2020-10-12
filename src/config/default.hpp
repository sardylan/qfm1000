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

#ifndef __QFM1000_CONFIG_DEFAULT_H
#define __QFM1000_CONFIG_DEFAULT_H

#if defined(Q_OS_WIN)
#define CONFIG_ARDUINO_PORT_NAME_DEFAULT "COM3"
#define CONFIG_RADIO_PORT_NAME_DEFAULT "COM4"
#else
#define CONFIG_ARDUINO_PORT_NAME_DEFAULT "/dev/ttyACM0"
#define CONFIG_RADIO_PORT_NAME_DEFAULT "/dev/ttyUSB0"
#endif

#define CONFIG_ARDUINO_PORT_SPEED_DEFAULT QSerialPort::Baud115200
#define CONFIG_RADIO_PORT_SPEED_DEFAULT QSerialPort::Baud115200

#endif
