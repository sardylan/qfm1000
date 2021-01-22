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
#include <QtCore/QList>
#include <QtCore/QString>

#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>

#include "config.hpp"
#include "ui_config.h"

using namespace qfm1000::app::windows;

Config::Config(QWidget *parent) : QDialog(parent), ui(new Ui::Config) {
    ui->setupUi(this);

    Config::currentConfig = new qfm1000::app::Config();
    Config::newConfig = new qfm1000::app::Config();

    initUi();
    load();
    connectSignals();
}

Config::~Config() {
    delete newConfig;
    delete currentConfig;

    delete ui;
}

void Config::initUi() {
    qInfo() << "Initializing UI";

    qDebug() << "Initializing InoProg SerialPort ComboBox";
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::iterator portIterator;

    ui->inoProgPortNameComboBox->clear();

    for (portIterator = portList.begin(); portIterator != portList.end(); portIterator++) {
        QSerialPortInfo serialPortInfo = *portIterator;
        const QString &systemLocation = serialPortInfo.systemLocation();
        ui->inoProgPortNameComboBox->addItem(systemLocation, systemLocation);
    }

    qDebug() << "Initializing InoProg SerialSpeed ComboBox";
    ui->inoProgPortSpeedComboBox->clear();
    ui->inoProgPortSpeedComboBox->addItem("1200 baud", QSerialPort::Baud1200);
    ui->inoProgPortSpeedComboBox->addItem("2400 baud", QSerialPort::Baud2400);
    ui->inoProgPortSpeedComboBox->addItem("4800 baud", QSerialPort::Baud4800);
    ui->inoProgPortSpeedComboBox->addItem("9600 baud", QSerialPort::Baud9600);
    ui->inoProgPortSpeedComboBox->addItem("19200 baud", QSerialPort::Baud19200);
    ui->inoProgPortSpeedComboBox->addItem("38400 baud", QSerialPort::Baud38400);
    ui->inoProgPortSpeedComboBox->addItem("57600 baud", QSerialPort::Baud57600);
    ui->inoProgPortSpeedComboBox->addItem("115200 baud", QSerialPort::Baud115200);
}

void Config::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Config::save);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Config::close);

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &Config::save);
    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &Config::close);
    connect(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this, &Config::load);

}

void Config::setCurrentConfig(app::Config *config) {
    qInfo() << "Setting current config";

    Config::currentConfig->update(config);
    Config::newConfig->update(config);

    load();
}

void Config::load() {
    qInfo() << "Loading config";

    qDebug() << "Loading InoProg SerialPort";
    int index = -1;
    for (int i = 0; i < ui->inoProgPortNameComboBox->count(); i++)
        if (ui->inoProgPortNameComboBox->itemText(i) == currentConfig->getInoProg()->getPortName()) {
            index = i;
            break;
        }

    if (index != -1) {
        ui->inoProgPortNameComboBox->setCurrentIndex(index);
    } else {
        ui->inoProgPortNameComboBox->setItemText(0, currentConfig->getInoProg()->getPortName());
        ui->inoProgPortNameComboBox->setCurrentIndex(0);
    }

    qDebug() << "Loading InoProg SerialSpeed";
    for (int i = 0; i < ui->inoProgPortSpeedComboBox->count(); i++)
        if (ui->inoProgPortSpeedComboBox->itemData(i).value<QSerialPort::BaudRate>() ==
            currentConfig->getInoProg()->getPortSpeed()) {
            ui->inoProgPortSpeedComboBox->setCurrentIndex(i);
            break;
        }
}

void Config::save() {
    qInfo() << "Saving config";

    qDebug() << "Saving InoProg Serial Port and Speed";
    newConfig->getInoProg()->setPortName(ui->inoProgPortNameComboBox->currentData().value<QString>());
    newConfig->getInoProg()->setPortSpeed(ui->inoProgPortSpeedComboBox->currentData().value<QSerialPort::BaudRate>());

    qDebug() << "Emitting updateConfig signal";
    QMetaObject::invokeMethod(this, "updateConfig", Qt::QueuedConnection, Q_ARG(qfm1000::app::Config*, newConfig));

    qDebug() << "Updating current configuration";
    currentConfig->update(newConfig);
}
