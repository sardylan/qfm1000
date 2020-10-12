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

#include "config.hpp"
#include "default.hpp"

using namespace qfm1000::config;

Config *Config::instance = nullptr;

Config *Config::getInstance() {
    if (instance == nullptr)
        instance = new Config();

    return instance;
}

Config::Config() {
    arduinoPortName = CONFIG_ARDUINO_PORT_NAME_DEFAULT;
    arduinoPortSpeed = CONFIG_ARDUINO_PORT_SPEED_DEFAULT;
    radioPortName = CONFIG_RADIO_PORT_NAME_DEFAULT;
    radioPortSpeed = CONFIG_RADIO_PORT_SPEED_DEFAULT;
}

const QString &Config::getArduinoPortName() const {
    return arduinoPortName;
}

void Config::setArduinoPortName(const QString &arduinoPortName) {
    Config::arduinoPortName = arduinoPortName;
}

QSerialPort::BaudRate Config::getArduinoPortSpeed() const {
    return arduinoPortSpeed;
}

void Config::setArduinoPortSpeed(QSerialPort::BaudRate arduinoPortSpeed) {
    Config::arduinoPortSpeed = arduinoPortSpeed;
}

const QString &Config::getRadioPortName() const {
    return radioPortName;
}

void Config::setRadioPortName(const QString &radioPortName) {
    Config::radioPortName = radioPortName;
}

QSerialPort::BaudRate Config::getRadioPortSpeed() const {
    return radioPortSpeed;
}

void Config::setRadioPortSpeed(QSerialPort::BaudRate radioPortSpeed) {
    Config::radioPortSpeed = radioPortSpeed;
}
