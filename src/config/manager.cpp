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

#include <QtCore/QSettings>
#include <QSerialPort>

#include "manager.hpp"
#include "config.hpp"
#include "default.hpp"

Q_DECLARE_METATYPE(QSerialPort::BaudRate);

void ConfigManager::load() {
    Config *config = Config::getInstance();
    QSettings settings;

    settings.beginGroup("eeprom");
    config->setArduinoPortName(settings.value("portName", CONFIG_ARDUINO_PORT_NAME_DEFAULT).toString());
    config->setArduinoPortSpeed(
            settings.value("portSpeed", CONFIG_ARDUINO_PORT_SPEED_DEFAULT).value<QSerialPort::BaudRate>());
    settings.endGroup();

    settings.beginGroup("radio");
    config->setRadioPortName(settings.value("portName", CONFIG_RADIO_PORT_NAME_DEFAULT).toString());
    config->setRadioPortSpeed(
            settings.value("portSpeed", CONFIG_RADIO_PORT_SPEED_DEFAULT).value<QSerialPort::BaudRate>());
    settings.endGroup();
}

void ConfigManager::save() {
    Config *config = Config::getInstance();
    QSettings settings;

    settings.beginGroup("eeprom");
    settings.setValue("portName", config->getArduinoPortName());
    settings.setValue("portSpeed", config->getArduinoPortSpeed());
    settings.endGroup();

    settings.beginGroup("radio");
    settings.setValue("portName", config->getRadioPortName());
    settings.setValue("portSpeed", config->getRadioPortSpeed());
    settings.endGroup();

    settings.sync();
}
