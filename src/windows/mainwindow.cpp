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
#include <QtCore/QThread>

#include "mainwindow.hpp"
#include "ui_mainwindow.h"
#include "model/helper.hpp"

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
    squelchDelegate = new SquelchDelegate(this);
    powerDelegate = new PowerDelegate(this);
    selCalFlagDelegate = new FlagDelegate(this);
    cpuOffsetFlagDelegate = new FlagDelegate(this);

    signalConnect();
    initUi();
    initStatusBar();

    eepromUpdated();
    configUpdated();
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::signalConnect() {
    connect(ui->actionFileClose, &QAction::triggered, this, &MainWindow::closeFile);
    connect(ui->actionFileOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionFileSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->actionFileSaveas, &QAction::triggered, this, &MainWindow::saveFileAs);
    connect(ui->actionFileQuit, &QAction::triggered, this, &MainWindow::applicationClose);

    connect(ui->actionEditConfiguration, &QAction::triggered, this, &MainWindow::showConfigWindow);

    connect(ui->actionEepromRead, &QAction::triggered, this, &MainWindow::eepromRead);
    connect(ui->actionEepromWrite, &QAction::triggered, this, &MainWindow::eepromWrite);

    connect(ui->actionHelpAbout, &QAction::triggered, this, &MainWindow::showAboutWindow);

    connect(tableModel, &TableModel::dataChanged, this, &MainWindow::updateUiStatus);

    connect(ui->frequencyBandComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::valueWriteFrequencyBand);

    connect(ui->totSlider, &QSlider::valueChanged, this, &MainWindow::valueWriteTot);
    connect(ui->defaultChannelComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::valueWriteDefaultChannel);
    connect(ui->lowPowerComboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &MainWindow::valueWriteLowPower);
}

void MainWindow::initUi() {
    ui->actionEditCut->setEnabled(false);
    ui->actionEditCopy->setEnabled(false);
    ui->actionEditPaste->setEnabled(false);

    ui->tableView->setModel(tableModel);
    ui->tableView->setItemDelegateForColumn(2, offsetDelegate);
    ui->tableView->setItemDelegateForColumn(3, rxCtcssDelegate);
    ui->tableView->setItemDelegateForColumn(4, txCtcssDelegate);
    ui->tableView->setItemDelegateForColumn(5, squelchDelegate);
    ui->tableView->setItemDelegateForColumn(6, powerDelegate);
    ui->tableView->setItemDelegateForColumn(7, selCalFlagDelegate);
    ui->tableView->setItemDelegateForColumn(8, cpuOffsetFlagDelegate);
    ui->tableView->resizeColumnsToContents();

    ui->frequencyBandComboBox->clear();
    ui->frequencyBandComboBox->addItem(frequencyBandValues[FrequencyBand::B0], FrequencyBand::B0);
    ui->frequencyBandComboBox->addItem(frequencyBandValues[FrequencyBand::A9], FrequencyBand::A9);
    ui->frequencyBandComboBox->addItem(frequencyBandValues[FrequencyBand::TM], FrequencyBand::TM);
    ui->frequencyBandComboBox->addItem(frequencyBandValues[FrequencyBand::T4], FrequencyBand::T4);
    ui->frequencyBandComboBox->addItem(frequencyBandValues[FrequencyBand::U0], FrequencyBand::U0);

    ui->defaultChannelComboBox->clear();
    ui->defaultChannelComboBox->addItem("Last used", -1);
    for (int i = 0; i < CHANNELS_COUNT; i++)
        ui->defaultChannelComboBox->addItem(QString("%1").arg(i), i);

    ui->lowPowerComboBox->clear();
    for (int i = 0; i < powerValues.size(); i++)
        ui->lowPowerComboBox->addItem(powerValues[i], i);

    valueReadDefaultChannel();
    valueReadLowPower();
    updateTotValueString();
}

void MainWindow::initStatusBar() {
    ui->statusBar->addPermanentWidget(statusBarWidgets->arduinoSerial);

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

    int result = fileDialog.exec();
    if (result != QDialog::Accepted)
        return;

    QStringList selectedFiles = fileDialog.selectedFiles();
    if (selectedFiles.size() != 1)
        return;

    emit actionLoadFile(selectedFiles[0]);
}

void MainWindow::saveFile() {
    emit actionSaveFile(status->getCurrentFileName());
}

void MainWindow::saveFileAs() {
    QFileDialog fileDialog;
    fileDialog.setAcceptMode(QFileDialog::AcceptSave);
    fileDialog.setFileMode(QFileDialog::AnyFile);

    int result = fileDialog.exec();
    if (result != QDialog::Accepted)
        return;

    QStringList selectedFiles = fileDialog.selectedFiles();
    if (selectedFiles.size() != 1)
        return;

    emit actionSaveFile(selectedFiles[0]);
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

void MainWindow::updateTotValueString() {
    if (eeprom->getTot() == 0)
        ui->totText->setText("DISABLED");
    else
        ui->totText->setText(QString("%1 s").arg(eeprom->getTot()));
}

void MainWindow::valueWriteTot() {
    eeprom->setTot(ui->totSlider->value());
    updateTotValueString();
    updateUiStatus();
}

void MainWindow::valueWriteDefaultChannel(int newValue) {
    eeprom->setDefaultChannel(ui->defaultChannelComboBox->itemData(newValue).toInt());
    updateUiStatus();
}

void MainWindow::valueWriteLowPower(int newValue) {
    eeprom->setLowPower(ui->lowPowerComboBox->itemData(newValue).toUInt());
    updateUiStatus();
}

void MainWindow::valueWriteFrequencyBand(int newValue) {
    status->setFrequencyBand((FrequencyBand) ui->frequencyBandComboBox->itemData(newValue).toInt());
    ui->tableView->viewport()->repaint();
    updateUiStatus();
}

void MainWindow::valueReadDefaultChannel() {
    ui->defaultChannelComboBox->setCurrentIndex(ui->defaultChannelComboBox->findData(eeprom->getDefaultChannel()));
}

void MainWindow::valueReadLowPower() {
    ui->lowPowerComboBox->setCurrentIndex(ui->lowPowerComboBox->findData(eeprom->getLowPower()));
}

void MainWindow::valueReadFrequencyBand() {
    ui->frequencyBandComboBox->setCurrentIndex(ui->frequencyBandComboBox->findData(status->getFrequencyBand()));
}

void MainWindow::eepromUpdated() {
    updateUiStatus();
    updateWidgetEnableStatus();

    bool fileOpened = status->getCurrentFileName().length() > 0;
    if (!fileOpened)
        return;

    ui->tableView->update();

    ui->totSlider->setValue(eeprom->getTot());
    updateTotValueString();

    valueReadDefaultChannel();
    valueReadLowPower();
    valueReadFrequencyBand();
}

void MainWindow::configUpdated() {
    statusBarWidgets->updateFromConfig();
}

void MainWindow::updateWidgetEnableStatus() {
    bool fileOpened = status->getCurrentFileName().length() > 0;
    ui->generalConfGroupBox->setEnabled(fileOpened);
    ui->channelsGroupBox->setEnabled(fileOpened);
    ui->eepromFeaturesGroupBox->setEnabled(fileOpened);
}

void MainWindow::updateUiStatus() {
    bool fileOpened = status->getCurrentFileName().length() > 0;
    bool eepromDirty = status->isDataDirty(eeprom->getData());

    ui->actionFileClose->setEnabled(fileOpened);
    ui->actionFileSave->setEnabled(fileOpened && eepromDirty);
    ui->actionFileSaveas->setEnabled(fileOpened);

    QString title = QString("%1 %2")
            .arg(QCoreApplication::applicationName())
            .arg(QCoreApplication::applicationVersion());

    if (fileOpened) {
        QString fileName = status->getCurrentFileName();
        title.append(QString(" - %1").arg(fileName));

        if (eepromDirty)
            title.append("*");
    }

    setWindowTitle(title);

    statusBarWidgets->updateFromConfig();
}

void MainWindow::eepromRead() {
    emit actionEepromRead();
}

void MainWindow::eepromWrite() {
    emit actionEepromWrite();
}
