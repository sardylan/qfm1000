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
    inoProg->setBaudRate(config->getInoProg()->getBaudRate());
    inoProg->setDataBits(config->getInoProg()->getDataBits());
    inoProg->setParity(config->getInoProg()->getParity());
    inoProg->setStopBits(config->getInoProg()->getStopBits());
    inoProg->setFlowControl(config->getInoProg()->getFlowControl());
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

QSerialPort::BaudRate Config::InoProg::getBaudRate() const {
    return baudRate;
}

void Config::InoProg::setBaudRate(QSerialPort::BaudRate newValue) {
    InoProg::baudRate = newValue;
}

QSerialPort::DataBits Config::InoProg::getDataBits() const {
    return dataBits;
}

void Config::InoProg::setDataBits(QSerialPort::DataBits newValue) {
    InoProg::dataBits = newValue;
}

QSerialPort::Parity Config::InoProg::getParity() const {
    return parity;
}

void Config::InoProg::setParity(QSerialPort::Parity newValue) {
    InoProg::parity = newValue;
}

QSerialPort::StopBits Config::InoProg::getStopBits() const {
    return stopBits;
}

void Config::InoProg::setStopBits(QSerialPort::StopBits newValue) {
    InoProg::stopBits = newValue;
}

QSerialPort::FlowControl Config::InoProg::getFlowControl() const {
    return flowControl;
}

void Config::InoProg::setFlowControl(QSerialPort::FlowControl newValue) {
    InoProg::flowControl = newValue;
}

void Config::load() {
    qInfo() << "Loading config from system";

    QSettings settings;

    settings.beginGroup(CONFIG_INOPROG_TAG);
    inoProg->setPortName(settings.value(CONFIG_SERIALPORT_PORTNAME_TAG,
                                        CONFIG_INOPROG_PORTNAME_DEFAULT).toString());
    inoProg->setBaudRate(settings.value(CONFIG_SERIALPORT_BAUDRATE_TAG,
                                        CONFIG_INOPROG_BAUDRATE_DEFAULT).value<QSerialPort::BaudRate>());
    inoProg->setDataBits(settings.value(CONFIG_SERIALPORT_DATABITS_TAG,
                                        CONFIG_INOPROG_DATABITS_DEFAULT).value<QSerialPort::DataBits>());
    inoProg->setParity(settings.value(CONFIG_SERIALPORT_PARITY_TAG,
                                      CONFIG_INOPROG_PARITY_DEFAULT).value<QSerialPort::Parity>());
    inoProg->setStopBits(settings.value(CONFIG_SERIALPORT_STOPBITS_TAG,
                                        CONFIG_INOPROG_STOPBITS_DEFAULT).value<QSerialPort::StopBits>());
    inoProg->setFlowControl(settings.value(CONFIG_SERIALPORT_FLOWCONTROL_TAG,
                                           CONFIG_INOPROG_FLOWCONTROL_DEFAULT).value<QSerialPort::FlowControl>());
    settings.endGroup();
}

void Config::save() {
    qInfo() << "saving config to system";

    QSettings settings;

    settings.beginGroup(CONFIG_INOPROG_TAG);
    settings.setValue(CONFIG_SERIALPORT_PORTNAME_TAG, inoProg->getPortName());
    settings.setValue(CONFIG_SERIALPORT_BAUDRATE_TAG, inoProg->getBaudRate());
    settings.setValue(CONFIG_SERIALPORT_DATABITS_TAG, inoProg->getDataBits());
    settings.setValue(CONFIG_SERIALPORT_PARITY_TAG, inoProg->getParity());
    settings.setValue(CONFIG_SERIALPORT_STOPBITS_TAG, inoProg->getStopBits());
    settings.setValue(CONFIG_SERIALPORT_FLOWCONTROL_TAG, inoProg->getFlowControl());
    settings.endGroup();
}

void Config::registerMetaTypes() {
    qRegisterMetaType<qfm1000::app::Config>("qfm1000::app::Config");
    qRegisterMetaType<qfm1000::app::Config *>("qfm1000::app::Config*");
}
