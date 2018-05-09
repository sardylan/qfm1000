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

#include "arduinowindow.hpp"
#include "ui_arduinowindow.h"

#include <QtWidgets>

ArduinoWindow::ArduinoWindow(QWidget *parent) : QDialog(parent), ui(new Ui::ArduinoWindow) {
    ui->setupUi(this);

    signalConnect();
    initUi();
}

ArduinoWindow::~ArduinoWindow() {
    delete ui;
}

void ArduinoWindow::signalConnect() {
    connect(ui->buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)), this, SLOT(handleClose()));
}

void ArduinoWindow::initUi() {

}

void ArduinoWindow::handleClose() {
    close();
}

void ArduinoWindow::start() {
    ui->buttonBox->button(QDialogButtonBox::Close)->setEnabled(false);
    ui->logText->clear();
    ui->progressBar->setMaximum(10);
    ui->progressBar->setValue(0);
}

void ArduinoWindow::finish() {
    ui->buttonBox->button(QDialogButtonBox::Close)->setEnabled(true);
    ui->progressBar->setMaximum(10);
    ui->progressBar->setValue(10);
}

void ArduinoWindow::log(QString text) {
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString line = QString("<strong>%1</strong> - %2\n").arg(dateTime).arg(text);
    ui->logText->append(line);
    ui->logText->moveCursor(QTextCursor::End);
}

void ArduinoWindow::progress(int maximum, int value) {
    ui->progressBar->setMaximum(maximum);
    ui->progressBar->setValue(value);
}
