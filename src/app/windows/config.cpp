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
#include <QtWidgets/QMessageBox>

#include "../default.hpp"

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

    qDebug() << "Initializing InoProg SerialPort ComboBoxes";
    initSerialPortWidgets(
            ui->inoProgPortNameComboBox,
            ui->inoProgBaudRateComboBox,
            ui->inoProgDataBitsComboBox,
            ui->inoProgParityComboBox,
            ui->inoProgStopBitsComboBox,
            ui->inoProgFlowControlComboBox
    );
}

void Config::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Config::save);
    connect(ui->buttonBox->button(QDialogButtonBox::Ok), &QPushButton::clicked, this, &Config::close);

    connect(ui->buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, this, &Config::save);
    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QPushButton::clicked, this, &Config::close);
    connect(ui->buttonBox->button(QDialogButtonBox::Reset), &QPushButton::clicked, this, &Config::load);
    connect(ui->buttonBox->button(QDialogButtonBox::RestoreDefaults), &QPushButton::clicked, this,
            &Config::restoreDefault);

}

void Config::setCurrentConfig(app::Config *config) {
    qInfo() << "Setting current config";

    Config::currentConfig->update(config);
    Config::newConfig->update(config);

    load();
}

void Config::load() {
    qInfo() << "Loading config";

    selectInitialPortNameValue(ui->inoProgPortNameComboBox, currentConfig->getInoProg()->getPortName());
    selectInitialBaudRateValue(ui->inoProgBaudRateComboBox, currentConfig->getInoProg()->getBaudRate());
    selectInitialDataBitsValue(ui->inoProgDataBitsComboBox, currentConfig->getInoProg()->getDataBits());
    selectInitialParityValue(ui->inoProgParityComboBox, currentConfig->getInoProg()->getParity());
    selectInitialStopBitsValue(ui->inoProgStopBitsComboBox, currentConfig->getInoProg()->getStopBits());
    selectInitialFlowControlValue(ui->inoProgFlowControlComboBox, currentConfig->getInoProg()->getFlowControl());
}

void Config::save() {
    qInfo() << "Saving config";

    qDebug() << "Saving InoProg Serial Port params";
    newConfig->getInoProg()->setPortName(ui->inoProgPortNameComboBox->currentData().value<QString>());
    newConfig->getInoProg()->setBaudRate(ui->inoProgBaudRateComboBox->currentData().value<QSerialPort::BaudRate>());
    newConfig->getInoProg()->setDataBits(ui->inoProgDataBitsComboBox->currentData().value<QSerialPort::DataBits>());
    newConfig->getInoProg()->setParity(ui->inoProgParityComboBox->currentData().value<QSerialPort::Parity>());
    newConfig->getInoProg()->setStopBits(ui->inoProgStopBitsComboBox->currentData().value<QSerialPort::StopBits>());
    newConfig->getInoProg()->setFlowControl(ui->inoProgFlowControlComboBox->currentData().value<QSerialPort::FlowControl>());

    qDebug() << "Emitting updateConfig signal";
    QMetaObject::invokeMethod(this, "updateConfig", Qt::QueuedConnection, Q_ARG(qfm1000::app::Config*, newConfig));

    qDebug() << "Updating current configuration";
    currentConfig->update(newConfig);
}

void Config::restoreDefault() {
    qInfo() << "Restoring config to default values";

    qDebug() << "Asking for confirmation";
    if (QMessageBox::question(
            this,
            "Restore default values confirmation",
            "Are you sure to restore default values?"
    ) != QMessageBox::Yes)
        return;

    qDebug() << "Loading InoProg SerialPort params";
    selectInitialPortNameValue(ui->inoProgPortNameComboBox, CONFIG_INOPROG_PORTNAME_DEFAULT);
    selectInitialBaudRateValue(ui->inoProgBaudRateComboBox, CONFIG_INOPROG_BAUDRATE_DEFAULT);
    selectInitialDataBitsValue(ui->inoProgDataBitsComboBox, CONFIG_INOPROG_DATABITS_DEFAULT);
    selectInitialParityValue(ui->inoProgParityComboBox, CONFIG_INOPROG_PARITY_DEFAULT);
    selectInitialStopBitsValue(ui->inoProgStopBitsComboBox, CONFIG_INOPROG_STOPBITS_DEFAULT);
    selectInitialFlowControlValue(ui->inoProgFlowControlComboBox, CONFIG_INOPROG_FLOWCONTROL_DEFAULT);
}

void
Config::initSerialPortWidgets(
        QComboBox *portNameComboBox,
        QComboBox *baudRateComboBox,
        QComboBox *dataBitsComboBox,
        QComboBox *parityComboBox,
        QComboBox *stopBitsComboBox,
        QComboBox *flowControlComboBox
) {
    qInfo() << "Initializing serial port widgets";

    qDebug() << "initializing portName ComboBox";
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    QList<QSerialPortInfo>::iterator portIterator;
    portNameComboBox->clear();
    for (portIterator = portList.begin(); portIterator != portList.end(); portIterator++) {
        QSerialPortInfo serialPortInfo = *portIterator;
        const QString &systemLocation = serialPortInfo.systemLocation();
        portNameComboBox->addItem(systemLocation, systemLocation);
    }

    qDebug() << "initializing baudRate ComboBox";
    baudRateComboBox->clear();
    baudRateComboBox->addItem("1200 baud", QSerialPort::Baud1200);
    baudRateComboBox->addItem("2400 baud", QSerialPort::Baud2400);
    baudRateComboBox->addItem("4800 baud", QSerialPort::Baud4800);
    baudRateComboBox->addItem("9600 baud", QSerialPort::Baud9600);
    baudRateComboBox->addItem("19200 baud", QSerialPort::Baud19200);
    baudRateComboBox->addItem("38400 baud", QSerialPort::Baud38400);
    baudRateComboBox->addItem("57600 baud", QSerialPort::Baud57600);
    baudRateComboBox->addItem("115200 baud", QSerialPort::Baud115200);

    qDebug() << "initializing dataBits ComboBox";
    dataBitsComboBox->clear();
    dataBitsComboBox->addItem("5 bits", QSerialPort::Data5);
    dataBitsComboBox->addItem("6 bits", QSerialPort::Data6);
    dataBitsComboBox->addItem("7 bits", QSerialPort::Data7);
    dataBitsComboBox->addItem("8 bits", QSerialPort::Data8);

    qDebug() << "initializing parity ComboBox";
    parityComboBox->clear();
    parityComboBox->addItem("None", QSerialPort::NoParity);
    parityComboBox->addItem("Even", QSerialPort::EvenParity);
    parityComboBox->addItem("Odd", QSerialPort::OddParity);
    parityComboBox->addItem("Space", QSerialPort::SpaceParity);
    parityComboBox->addItem("Mark", QSerialPort::MarkParity);

    qDebug() << "initializing stopBits ComboBox";
    stopBitsComboBox->clear();
    stopBitsComboBox->addItem("1 bit", QSerialPort::OneStop);
    stopBitsComboBox->addItem("1.5 bits", QSerialPort::OneAndHalfStop);
    stopBitsComboBox->addItem("2 bits", QSerialPort::TwoStop);

    qDebug() << "initializing flowControl ComboBox";
    flowControlComboBox->clear();
    flowControlComboBox->addItem("None", QSerialPort::NoFlowControl);
    flowControlComboBox->addItem("Hardware (RTS-CTS)", QSerialPort::HardwareControl);
    flowControlComboBox->addItem("Software (XON-XOFF)", QSerialPort::SoftwareControl);
}

void Config::selectInitialPortNameValue(QComboBox *comboBox, const QString &initialValue) {
    qInfo() << "Selecting initial value for Serial Port Name";

    qDebug() << "Searching for index";
    int index = -1;
    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).toString() == initialValue) {
            index = i;
            break;
        }

    if (index != -1) {
        qDebug() << "Index found:" << index;
        comboBox->setCurrentIndex(index);
    } else {
        qDebug() << "Index not found. Adding current option";
        comboBox->setItemText(0, initialValue);
        comboBox->setItemData(0, initialValue);
        comboBox->setCurrentIndex(0);
    }
}

void Config::selectInitialBaudRateValue(QComboBox *comboBox, QSerialPort::BaudRate initialValue) {
    qInfo() << "Selecting initial value for Serial Port BaudRate";

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<QSerialPort::BaudRate>() == initialValue) {
            comboBox->setCurrentIndex(i);
            break;
        }
}

void Config::selectInitialDataBitsValue(QComboBox *comboBox, QSerialPort::DataBits initialValue) {
    qInfo() << "Selecting initial value for Serial Port DataBits";

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<QSerialPort::DataBits>() == initialValue) {
            comboBox->setCurrentIndex(i);
            break;
        }
}

void Config::selectInitialParityValue(QComboBox *comboBox, QSerialPort::Parity initialValue) {
    qInfo() << "Selecting initial value for Serial Port Parity";

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<QSerialPort::Parity>() == initialValue) {
            comboBox->setCurrentIndex(i);
            break;
        }
}

void Config::selectInitialStopBitsValue(QComboBox *comboBox, QSerialPort::StopBits initialValue) {
    qInfo() << "Selecting initial value for Serial Port StopBits";

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<QSerialPort::StopBits>() == initialValue) {
            comboBox->setCurrentIndex(i);
            break;
        }
}

void Config::selectInitialFlowControlValue(QComboBox *comboBox, QSerialPort::FlowControl initialValue) {
    qInfo() << "Selecting initial value for Serial Port FlowControl";

    for (int i = 0; i < comboBox->count(); i++)
        if (comboBox->itemData(i).value<QSerialPort::FlowControl>() == initialValue) {
            comboBox->setCurrentIndex(i);
            break;
        }
}
