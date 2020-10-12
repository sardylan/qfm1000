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

#include "aboutwindow.hpp"
#include "ui_aboutwindow.h"

#include <QtWidgets>

using namespace qfm1000::windows;

AboutWindow::AboutWindow(QWidget *parent) : QDialog(parent), ui(new Ui::AboutWindow) {
    ui->setupUi(this);

    signalConnect();
    initUi();
}

AboutWindow::~AboutWindow() {
    delete ui;
}

void AboutWindow::signalConnect() {
    connect(ui->buttonBox->button(QDialogButtonBox::Close), SIGNAL(clicked(bool)), this, SLOT(handleClose()));
}

void AboutWindow::initUi() {
    QFont titleFont = ui->titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(18);
    ui->titleLabel->setFont(titleFont);
    ui->titleLabel->setText(QString("%1\n%2")
                                    .arg(QCoreApplication::applicationName())
                                    .arg(QCoreApplication::applicationVersion()));
}

void AboutWindow::handleClose() {
    close();
}
