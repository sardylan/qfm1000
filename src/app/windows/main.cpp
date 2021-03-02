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
#include <QtCore/QMap>
#include <QtCore/QDateTime>

#include <utilities/serialport.hpp>

#include "main.hpp"
#include "ui_main.h"

using namespace qfm1000;
using namespace qfm1000::app::windows;

Main::Main(QWidget *parent) : QMainWindow(parent), ui(new Ui::Main) {
    ui->setupUi(this);

    instanceWindows = new QMap<quint64, Instance *>();

    clockTimer = new QTimer();
    statusBarClockLabel = new QLabel();
    statusBarSerialPortLabel = new QLabel();

    connectSignals();
    initUi();
}

Main::~Main() {
    delete statusBarSerialPortLabel;
    delete statusBarClockLabel;
    delete clockTimer;

    delete instanceWindows;

    delete ui;
}

void Main::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->actionAbout, &QAction::triggered, this, &Main::displayAbout, Qt::QueuedConnection);
    connect(ui->actionLicense, &QAction::triggered, this, &Main::displayLicense, Qt::QueuedConnection);

    connect(ui->actionConfiguration, &QAction::triggered, this, &Main::actionConfiguration, Qt::QueuedConnection);

    connect(ui->actionFileOpen, &QAction::triggered, this, &Main::actionFileOpen, Qt::QueuedConnection);
}

void Main::initUi() {
    qInfo() << "Initalizing UI";

    updateWindowTitle();

    initStatusBar();
}

void Main::initStatusBar() {
    qInfo() << "Initalizing Status Bar";

    qDebug() << "Initalizing Serial Port Name widget";
    ui->statusBar->addPermanentWidget(statusBarSerialPortLabel);
    applyStatusBarLabelStyle(statusBarSerialPortLabel, "InoProg serial port");
    statusBarSerialPortLabel->setEnabled(false);

    qDebug() << "Initalizing Clock widget";
    ui->statusBar->addPermanentWidget(statusBarClockLabel);
    applyStatusBarLabelStyle(statusBarClockLabel);
    statusBarClockLabel->setStyleSheet("");
    clockTimer->setInterval(1000);
    clockTimer->setSingleShot(false);
    connect(clockTimer, &QTimer::timeout, this, &Main::updateStatusBarClock, Qt::QueuedConnection);
    clockTimer->start();
    QMetaObject::invokeMethod(this, "updateStatusBarClock", Qt::QueuedConnection);
}

void Main::updateStatusBarClock() {
    statusBarClockLabel->setText(QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
}

void Main::updateWindowTitle() {
    qInfo() << "Updating window title";

    setWindowTitle(QString("%1 %2")
                           .arg(QCoreApplication::applicationName())
                           .arg(QCoreApplication::applicationVersion()));
}

void Main::addInstance(quint64 id, Instance *window) {
    qInfo() << "Adding instance window with id" << id;

    instanceWindows->insert(id, window);
    ui->mdiArea->addSubWindow(window);
    window->showMaximized();
}

void Main::removeInstance(quint64 id) {
    qInfo() << "Remove instance window with id" << id;

    Instance *instance = instanceWindows->value(id);
    ui->mdiArea->removeSubWindow(instance);
    instanceWindows->remove(id);
}

void Main::updateSerialPortLabel(
        const QString &portName,
        const QSerialPort::BaudRate &baudRate,
        const QSerialPort::DataBits &dataBits,
        const QSerialPort::Parity &parity,
        const QSerialPort::StopBits &stopBits,
        const QSerialPort::FlowControl &flowControl
) {
    QString serialPortText = utilities::SerialPort::prettyConfig(
            portName,
            baudRate,
            dataBits,
            parity,
            stopBits,
            flowControl
    );
    statusBarSerialPortLabel->setText(serialPortText);
}

void Main::updateSerialPortWorking(bool working) {
    statusBarSerialPortLabel->setEnabled(working);
}

void Main::applyStatusBarLabelStyle(QLabel *label, const QString &toolTip, const QString &defaultText) {
    label->setStyleSheet("margin-right: 10px");

    if (toolTip.size() > 0)
        label->setToolTip(toolTip);

    if (defaultText.size() > 0)
        label->setText(defaultText);
}
