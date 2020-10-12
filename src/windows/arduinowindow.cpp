/*
 * qFM1000
 * Copyright (C) 2020  Luca Cireddu (sardylan@gmail.com)
 * https://www.lucacireddu.it/
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
    connect(ui->buttonBox->button(QDialogButtonBox::Close),
            &QAbstractButton::clicked,
            this,
            &ArduinoWindow::close);
}

void ArduinoWindow::initUi() {

}

void ArduinoWindow::start() {
    ui->buttonBox->button(QDialogButtonBox::Close)->setEnabled(false);
    ui->logText->clear();
    ui->progressBar->setMaximum(255);
    ui->progressBar->setValue(0);
}

void ArduinoWindow::finish() {
    ui->buttonBox->button(QDialogButtonBox::Close)->setEnabled(true);
    ui->progressBar->setMaximum(255);
    ui->progressBar->setValue(255);
}

void ArduinoWindow::log(const QString& text) {
    QString dateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    QString line = QString("<strong>%1</strong> - %2\n").arg(dateTime).arg(text);
    ui->logText->append(line);
    ui->logText->moveCursor(QTextCursor::End);
}

void ArduinoWindow::progress(quint8 value) {
    ui->progressBar->setValue(value);
}
