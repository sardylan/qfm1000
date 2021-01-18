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
#include <QtCore/QCoreApplication>

#include "main.hpp"
#include "ui_main.h"

using namespace qfm1000::app::windows;

Main::Main(QWidget *parent) : QMainWindow(parent), ui(new Ui::Main) {
    ui->setupUi(this);

    connectSignals();
    initUi();
}

Main::~Main() {
    delete ui;
}

void Main::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->actionAbout, &QAction::triggered, this, &Main::displayAbout);
}

void Main::initUi() {
    qInfo() << "Initalizing UI";

    updateWindowTitle();
}

void Main::updateWindowTitle() {
    qInfo() << "Updating window title";

    setWindowTitle(QString("%1 %2")
                           .arg(QCoreApplication::applicationName())
                           .arg(QCoreApplication::applicationVersion()));
}
