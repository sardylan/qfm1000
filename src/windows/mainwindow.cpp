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

#include <QDebug>
#include <QOpenGLWidget>
#include <QtWidgets/QFileDialog>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    statusBarWidgets = new StatusBarWidgets(this);

    status = Status::getInstance();
    config = Config::getInstance();

    eeprom = EEPROM::getInstance();

    tableModel = new TableModel(this);

    offsetDelegate = new ReadOnlyDelegate(this);
    rxCtcssDelegate = new CtcssDelegate(this);
    txCtcssDelegate = new CtcssDelegate(this);
    powerDelegate = new PowerDelegate(this);
    selCalFlagDelegate = new FlagDelegate(this);
    cpuOffsetFlagDelegate = new FlagDelegate(this);

    signalConnect();
    initUi();
    initStatusBar();

    eepromUpdated();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::signalConnect() {
    connect(ui->actionFileClose, SIGNAL(triggered()), this, SLOT(closeFile()));
    connect(ui->actionFileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionFileSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionFileQuit, SIGNAL(triggered()), this, SLOT(applicationClose()));

    connect(ui->actionHelpAbout, SIGNAL(triggered()), this, SLOT(showAboutWindow()));

    connect(ui->totSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTotValue()));
    connect(ui->defaultChannelComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDefaultChannelValue(int)));
    connect(ui->lowPowerComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateLowPowerValue(int)));
}

void MainWindow::initUi() {
    ui->tableView->setModel(tableModel);
    ui->tableView->setItemDelegateForColumn(2, offsetDelegate);
    ui->tableView->setItemDelegateForColumn(3, rxCtcssDelegate);
    ui->tableView->setItemDelegateForColumn(4, txCtcssDelegate);
    ui->tableView->setItemDelegateForColumn(5, powerDelegate);
    ui->tableView->setItemDelegateForColumn(6, selCalFlagDelegate);
    ui->tableView->setItemDelegateForColumn(7, cpuOffsetFlagDelegate);
    ui->tableView->resizeColumnsToContents();

    ui->defaultChannelComboBox->clear();
    ui->defaultChannelComboBox->addItem("Last used", -1);
    for (int i = 0; i < CHANNELS_COUNT; i++)
        ui->defaultChannelComboBox->addItem(QString("%1").arg(i), i);

    ui->lowPowerComboBox->clear();
    ui->lowPowerComboBox->addItem("1 W", 1);
    ui->lowPowerComboBox->addItem("6 W", 2);
    ui->lowPowerComboBox->addItem("10 W", 3);
    ui->lowPowerComboBox->addItem("15 W", 4);

    setDefaultChannelValue();
    setLowPowerValue();
    updateTotValueString();
}

void MainWindow::initStatusBar() {
    ui->statusBar->addPermanentWidget(statusBarWidgets->time);
}

void MainWindow::applicationClose() {
    QApplication::quit();
}

void MainWindow::closeFile() {
    emit actionCloseFile();
}

void MainWindow::openFile() {
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptOpen);
    fileDialog.setFileMode(QFileDialog::ExistingFile);
    fileDialog.setOption(QFileDialog::ReadOnly, true);
    fileDialog.exec();

    QString fileName = fileDialog.selectedFiles().at(0);

    emit actionLoadFile(fileName);
}

void MainWindow::saveFile() {
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);
    fileDialog.exec();

    QString fileName = fileDialog.selectedFiles().at(0);

    emit actionSaveFile(fileName);
}

void MainWindow::showStatusBarMessage(QString message) {
    ui->statusBar->showMessage(message, 5000);
}

void MainWindow::showConfigWindow() {
    emit actionConfig();
}

void MainWindow::showAboutWindow() {
    emit actionAbout();
}

void MainWindow::updateTotValue() {
    eeprom->setTot(ui->totSlider->value());
    updateTotValueString();
}

void MainWindow::updateTotValueString() {
    if (eeprom->getTot() == 0)
        ui->totText->setText("DISABLED");
    else
        ui->totText->setText(QString("%1 s").arg(eeprom->getTot()));
}

void MainWindow::updateDefaultChannelValue(int newValue) {
    eeprom->setDefaultChannel(ui->defaultChannelComboBox->itemData(newValue).toInt());
}

void MainWindow::updateLowPowerValue(int newValue) {
    eeprom->setLowPower(ui->lowPowerComboBox->itemData(newValue).toInt());
}

void MainWindow::setDefaultChannelValue() {
    ui->defaultChannelComboBox->setCurrentIndex(ui->defaultChannelComboBox->findData(eeprom->getDefaultChannel()));
}

void MainWindow::setLowPowerValue() {
    ui->lowPowerComboBox->setCurrentIndex(ui->lowPowerComboBox->findData(eeprom->getLowPower()));
}

void MainWindow::eepromUpdated() {
    updateWindowFileName();

    if (status->getCurrentFileName().length() == 0) {
        widgetEnabled(false);
        return;
    }

    widgetEnabled(true);

    ui->tableView->update();

    ui->totSlider->setValue(eeprom->getTot());
    updateTotValueString();

    setDefaultChannelValue();
    setLowPowerValue();
}

void MainWindow::updateWindowFileName() {
    QString title = QString("%1\n%2")
            .arg(QCoreApplication::applicationName())
            .arg(QCoreApplication::applicationVersion());

    if (status->getCurrentFileName().length() > 0) {
        QString fileName = status->getCurrentFileName();
        title.append(QString(" - %1").arg(fileName));

        if (status->isDirty())
            title.append("*");
    }

    setWindowTitle(title);
}

void MainWindow::widgetEnabled(bool status) {
    ui->generalConfGroupBox->setEnabled(status);
    ui->channelsGroupBox->setEnabled(status);
}