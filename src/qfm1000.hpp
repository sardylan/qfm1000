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

#ifndef __QFM1000_QFM1000_H
#define __QFM1000_QFM1000_H

#include "version.hpp"

#include <QApplication>

#include <windows/mainwindow.hpp>

#include <config/config.hpp>
#include <config/status.hpp>
#include <eeprom/eeprom.hpp>
#include <programmer/programmer.hpp>


class QFM1000 : public QApplication {
Q_OBJECT

public:

    explicit QFM1000(int &argc, char **argv);

    ~QFM1000() override;

    void prepare();

    int run();

private:
    Status *status;
    Config *config;
    MainWindow *mainWindow;
    EEPROM *eeprom;
    ArduinoProgrammer *programmer;

private slots:

    void showConfigWindow();

    void showAboutWindow();

    void closeEepromFile();

    void loadEepromFile(QString fileName);

    void saveEepromFile(QString fileName);
};

#endif
