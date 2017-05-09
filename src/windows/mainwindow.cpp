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

    rxCtcssDelegate = new CtcssDelegate(this);
    txCtcssDelegate = new CtcssDelegate(this);

    signalConnect();
    initUi();
    initStatusBar();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::signalConnect() {
    connect(ui->actionFileOpen, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionFileSave, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionFileQuit, SIGNAL(triggered()), this, SLOT(applicationClose()));

    connect(ui->actionHelpAbout, SIGNAL(triggered()), this, SLOT(showAboutWindow()));

    connect(ui->totSlider, SIGNAL(valueChanged(int)), this, SLOT(updateTotValue(int)));
    connect(ui->defaultChannelComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateDefaultChannelValue(int)));
}

void MainWindow::initUi() {
    ui->tableView->setModel(tableModel);
    ui->tableView->setItemDelegateForColumn(4, rxCtcssDelegate);
    ui->tableView->setItemDelegateForColumn(5, txCtcssDelegate);

    ui->defaultChannelComboBox->clear();
    for (int i = 0; i < CHANNELS_COUNT; i++)
        ui->defaultChannelComboBox->addItem(QString("%1").arg(i), i);

    ui->defaultChannelComboBox->scr

    setDefaultChannelValue(0);
    updateTotValue(0);
}

void MainWindow::initStatusBar() {
    ui->statusBar->addPermanentWidget(statusBarWidgets->time);
}

void MainWindow::applicationClose() {
    QApplication::quit();
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

void MainWindow::updateTotValue(int newValue) {
    if (newValue == 0)
        ui->totText->setText("DISABLED");
    else
        ui->totText->setText(QString("%1 s").arg(newValue));

    eeprom->tot = (uint8_t) ui->totSlider->value();
}

void MainWindow::updateDefaultChannelValue(int newValue) {
    eeprom->defaultChannel = (uint8_t) ui->defaultChannelComboBox->itemData(newValue).toUInt();
}

void MainWindow::setDefaultChannelValue(uint8_t newValue) {
    ui->defaultChannelComboBox->setCurrentIndex(ui->defaultChannelComboBox->findData(newValue));
}

void MainWindow::eepromUpdated() {
    ui->tableView->update();
    updateTotValue(eeprom->tot);
    setDefaultChannelValue(eeprom->defaultChannel);
}
