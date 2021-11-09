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

#include "inoprog.hpp"
#include "ui_inoprog.h"

using namespace qfm1000;
using namespace qfm1000::app::windows;

InoProg::InoProg(QWidget *parent) : QDialog(parent), ui(new Ui::InoProg) {
    ui->setupUi(this);

    connectSignals();
    initUi();
}

InoProg::~InoProg() {
    delete ui;
}

void InoProg::connectSignals() {
    qInfo() << "Connecting signals";
}

void InoProg::initUi() {
    qInfo() << "Initalizing UI";
}
