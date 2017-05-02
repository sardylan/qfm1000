/*
 * qFM1000
 * Copyright (C) 2017  Luca Cireddu
 * sardylan@gmail.com
 * http://www.lucacireddu.it
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
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

#include "configwindow.hpp"
#include "ui_configwindow.h"

#include <QtWidgets>
#include <QSerialPortInfo>
#include <manager.hpp>

ConfigWindow::ConfigWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ConfigWindow) {
    ui->setupUi(this);

    status = Status::getInstance();
    config = Config::getInstance();

    signalConnect();
    initUi();
    initEnableStatus();
    load();
}

ConfigWindow::~ConfigWindow() {
    delete ui;
}

void ConfigWindow::signalConnect() {
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), SIGNAL(clicked(bool)), this, SLOT(handleOK()));
    connect(ui->buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked(bool)), this, SLOT(handleApply()));
    connect(ui->buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)), this, SLOT(handleClose()));
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), SIGNAL(clicked(bool)), this, SLOT(handleReset()));
}

void ConfigWindow::initUi() {
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::iterator portIterator;

    ui->serialPortCombo->clear();
    ui->serialPortCombo->addItem("");

    for (portIterator = portList.begin(); portIterator != portList.end(); portIterator++) {
        QSerialPortInfo serialPortInfo = *portIterator;
        ui->serialPortCombo->addItem(serialPortInfo.systemLocation());
    }

    ui->serialSpeedCombo->clear();
    ui->serialSpeedCombo->addItem("1200 baud", QSerialPort::Baud1200);
    ui->serialSpeedCombo->addItem("2400 baud", QSerialPort::Baud2400);
    ui->serialSpeedCombo->addItem("4800 baud", QSerialPort::Baud4800);
    ui->serialSpeedCombo->addItem("9600 baud", QSerialPort::Baud9600);
    ui->serialSpeedCombo->addItem("19200 baud", QSerialPort::Baud19200);
    ui->serialSpeedCombo->addItem("38400 baud", QSerialPort::Baud38400);
    ui->serialSpeedCombo->addItem("57600 baud", QSerialPort::Baud57600);
    ui->serialSpeedCombo->addItem("115200 baud", QSerialPort::Baud115200);
}

void ConfigWindow::initEnableStatus() {
    ui->serialPortCombo->setEnabled(!status->isSerialOpened());
    ui->serialSpeedCombo->setEnabled(!status->isSerialOpened());
}

void ConfigWindow::handleOK() {
    save();
    close();
}

void ConfigWindow::handleApply() {
    save();
}

void ConfigWindow::handleClose() {
    close();
}

void ConfigWindow::handleReset() {
    load();
}

void ConfigWindow::load() {
    ConfigManager::load();

    int index = -1;
    for (int i = 0; i < ui->serialPortCombo->count(); i++)
        if (ui->serialPortCombo->itemText(i) == config->getPortName()) {
            index = i;
            break;
        }

    if (index != -1) {
        ui->serialPortCombo->setCurrentIndex(index);
    } else {
        ui->serialPortCombo->setItemText(0, config->getPortName());
        ui->serialPortCombo->setCurrentIndex(0);
    }

    for (int i = 0; i < ui->serialSpeedCombo->count(); i++)
        if (ui->serialSpeedCombo->itemData(i).value<QSerialPort::BaudRate>() == config->getPortSpeed()) {
            ui->serialSpeedCombo->setCurrentIndex(i);
            break;
        }
}

void ConfigWindow::save() {
    config->setPortName(ui->serialPortCombo->currentText());
    config->setPortSpeed(ui->serialSpeedCombo->currentData().value<QSerialPort::BaudRate>());

    ConfigManager::save();
}