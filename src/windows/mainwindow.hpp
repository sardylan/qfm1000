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
#include "model/squelchdelegate.hpp"
#include "model/powerdelegate.hpp"
#include "model/flagdelegate.hpp"

using namespace qfm1000::config;

namespace Ui {
    class MainWindow;
}

namespace qfm1000::windows {

    class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);

        ~MainWindow() override;

    public slots:

        void configUpdated();

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
        SquelchDelegate *squelchDelegate;
        PowerDelegate *powerDelegate;
        FlagDelegate *selCalFlagDelegate;
        FlagDelegate *cpuOffsetFlagDelegate;

        void signalConnect();

        void initUi();

        void initStatusBar();

        void showStatusBarMessage(QString message);

        void valueReadDefaultChannel();

        void valueReadLowPower();

        void valueReadFrequencyBand();

        void updateWidgetEnableStatus();

        void updateTotValueString();

    private slots:

        void applicationClose();

        void newFile();

        void closeFile();

        void openFile();

        void saveFile();

        void saveFileAs();

        void showConfigWindow();

        void showAboutWindow();

        void updateUiStatus();

        void valueWriteTot();

        void valueWriteDefaultChannel(int newValue);

        void valueWriteLowPower(int newValue);

        void valueWriteFrequencyBand(int newValue);

        void eepromRead();

        void eepromWrite();

    signals:

        void actionConfig();

        void actionAbout();

        void actionNewFile();

        void actionCloseFile();

        void actionLoadFile(QString filename);

        void actionSaveFile(QString filename);

        void actionEepromRead();

        void actionEepromWrite();

    };

}

#endif
