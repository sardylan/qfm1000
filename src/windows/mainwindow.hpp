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

#ifndef __QFM1000_WINDOWS_MAINWINDOW_H
#define __QFM1000_WINDOWS_MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>

#include <config.hpp>
#include <status.hpp>

#include "widgets/statusbar.hpp"
#include "model/tablemodel.hpp"
#include "model/readonlydelegate.hpp"
#include "model/ctcssdelegate.hpp"
#include "model/powerdelegate.hpp"
#include "model/flagdelegate.hpp"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);

    ~MainWindow();

public slots:

    void eepromUpdated();

private:

    Ui::MainWindow *ui;

    Status *status;
    Config *config;

    EEPROM *eeprom;

    StatusBarWidgets *statusBarWidgets;
    TableModel *tableModel;

    ReadOnlyDelegate *offsetDelegate;
    CtcssDelegate *rxCtcssDelegate;
    CtcssDelegate *txCtcssDelegate;
    PowerDelegate *powerDelegate;
    FlagDelegate *selCalFlagDelegate;
    FlagDelegate *cpuOffsetFlagDelegate;

    void signalConnect();

    void initUi();

    void initStatusBar();

    void showStatusBarMessage(QString message);

    void setDefaultChannelValue(int newValue);

    void updateWindowFileName();

    void widgetEnabled(bool status);

private slots:

    void applicationClose();

    void closeFile();

    void openFile();

    void saveFile();

    void showConfigWindow();

    void showAboutWindow();

    void updateTotValue(int newValue);

    void updateDefaultChannelValue(int newValue);

signals:

    void actionConfig();

    void actionAbout();

    void actionCloseFile();

    void actionLoadFile(QString filename);

    void actionSaveFile(QString filename);

};

#endif
