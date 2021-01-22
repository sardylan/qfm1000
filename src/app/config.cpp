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


#include <QtCore/QDebug>
#include <QtCore/QSettings>

#include "config.hpp"
#include "default.hpp"

using namespace qfm1000::app;

Config::Config() {
    inoProg = new InoProg();
}

Config::~Config() {
    delete inoProg;
}

void Config::update(Config *config) {
    if (config == nullptr)
        return;

    qInfo() << "Updating config from new Config";

    inoProg->setPortName(config->getInoProg()->getPortName());
    inoProg->setPortSpeed(config->getInoProg()->getPortSpeed());
}

Config::InoProg *Config::getInoProg() const {
    return inoProg;
}

const QString &Config::InoProg::getPortName() const {
    return portName;
}

void Config::InoProg::setPortName(const QString &newValue) {
    InoProg::portName = newValue;
}

QSerialPort::BaudRate Config::InoProg::getPortSpeed() const {
    return portSpeed;
}

void Config::InoProg::setPortSpeed(QSerialPort::BaudRate newValue) {
    InoProg::portSpeed = newValue;
}

void Config::load() {
    qInfo() << "Loading config from system";

    QSettings settings;

    settings.beginGroup(CONFIG_INOPROG_TAG);
    inoProg->setPortName(settings.value(CONFIG_INOPROG_PORTNAME_TAG, CONFIG_INOPROG_PORTNAME_DEFAULT).toString());
    inoProg->setPortSpeed(settings.value(CONFIG_INOPROG_PORTSPEED_TAG,
                                         CONFIG_INOPROG_PORTSPEED_DEFAULT).value<QSerialPort::BaudRate>());
    settings.endGroup();
}

void Config::save() {
    qInfo() << "saving config to system";

    QSettings settings;

    settings.beginGroup(CONFIG_INOPROG_TAG);
    settings.setValue(CONFIG_INOPROG_PORTNAME_TAG, inoProg->getPortName());
    settings.setValue(CONFIG_INOPROG_PORTSPEED_TAG, inoProg->getPortSpeed());
    settings.endGroup();
}

void Config::registerMetaTypes() {
    qRegisterMetaType<qfm1000::app::Config>("qfm1000::app::Config");
    qRegisterMetaType<qfm1000::app::Config *>("qfm1000::app::Config*");
}
