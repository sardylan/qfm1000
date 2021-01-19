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

#include "eeprom.hpp"
#include "ui_eeprom.h"

using namespace qfm1000::app::windows;

EEPROM::EEPROM(QWidget *parent) : QWidget(parent), ui(new Ui::EEPROM) {
    ui->setupUi(this);

    connectSignals();
    initUi();
}

EEPROM::~EEPROM() {
    delete ui;
}

void EEPROM::connectSignals() {
    qInfo() << "Connecting signals";

}

void EEPROM::initUi() {
    qInfo() << "Initalizing UI";

}
