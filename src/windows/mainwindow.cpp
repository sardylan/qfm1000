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

#include "mainwindow.hpp"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    statusBarWidgets = new StatusBarWidgets();

    status = Status::getInstance();
    config = Config::getInstance();

    signalConnect();
    initUi();
    initStatusBar();
}

MainWindow::~MainWindow() {
    delete ui;
    delete statusBarWidgets;
}

void MainWindow::signalConnect() {
//    connect(ui->actionFileExit, SIGNAL(triggered()), this, SLOT(applicationClose()));
//    connect(ui->actionSetupConfig, SIGNAL(triggered()), this, SLOT(showConfigWindow()));
//    connect(ui->actionSetupRun, SIGNAL(triggered()), this, SLOT(toogleSerialPort()));
//    connect(ui->actionHelpAbout, SIGNAL(triggered()), this, SLOT(showAboutWindow()));
//
//    connect(ui->clearLineButton, SIGNAL(clicked()), this, SLOT(clearLine()));
//    connect(ui->clearTextButton, SIGNAL(clicked()), this, SLOT(clearText()));
//
//    connect(ui->timerIntervalSlider, SIGNAL(valueChanged(int)), this, SLOT(updateLineInterval(int)));
//
//    connect(ui->encoderSpeedSlider, SIGNAL(valueChanged(int)), this, SLOT(updateEncoderSpeed(int)));
//
//    connect(ui->playButton, SIGNAL(clicked()), this, SLOT(tooglePlayer()));
//    connect(ui->morseText, SIGNAL(selectionChanged()), this, SLOT(emitNewSelectedText()));
}

void MainWindow::initUi() {
}

void MainWindow::initStatusBar() {
    ui->statusBar->addPermanentWidget(statusBarWidgets->time);
}

void MainWindow::applicationClose() {
    QApplication::quit();
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
