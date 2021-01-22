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

#define CONFIG_INOPROG_TAG "InoProg"
#define CONFIG_INOPROG_PORTNAME_TAG "portName"
#define CONFIG_INOPROG_PORTSPEED_TAG "portSpeed"

#ifdef Q_OS_LINUX
#define CONFIG_INOPROG_PORTNAME_DEFAULT "/dev/ttyACM0"
#endif

#ifdef Q_OS_WINDOWS
#define CONFIG_INOPROG_PORTNAME_DEFAULT "COM3"
#endif

#define CONFIG_INOPROG_PORTSPEED_DEFAULT QSerialPort::Baud115200

#endif
