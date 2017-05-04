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
    connect(ui->actionFileQuit, SIGNAL(triggered()), this, SLOT(applicationClose()));

    connect(ui->actionHelpAbout, SIGNAL(triggered()), this, SLOT(showAboutWindow()));
}

void MainWindow::initUi() {
    ui->tableView->setModel(tableModel);
    ui->tableView->setItemDelegateForColumn(4, rxCtcssDelegate);
    ui->tableView->setItemDelegateForColumn(5, txCtcssDelegate);
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

void MainWindow::showStatusBarMessage(QString message) {
    ui->statusBar->showMessage(message, 5000);
}

void MainWindow::showConfigWindow() {
    emit actionConfig();
}

void MainWindow::showAboutWindow() {
    emit actionAbout();
}

void MainWindow::eepromUpdated() {
    ui->tableView->update();
}