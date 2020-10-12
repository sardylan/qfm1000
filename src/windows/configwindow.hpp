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

#ifndef __QFM1000_WINDOWS_CONFIGWINDOW_H
#define __QFM1000_WINDOWS_CONFIGWINDOW_H

#include <QDialog>

#include <config.hpp>
#include <status.hpp>

using namespace qfm1000::config;

namespace Ui {
    class ConfigWindow;
}

namespace qfm1000::windows {

    class ConfigWindow : public QDialog {
    Q_OBJECT

    public:

        explicit ConfigWindow(QWidget *parent = nullptr);

        ~ConfigWindow() override;

    private:

        Ui::ConfigWindow *ui;

        Status *status;
        Config *config;

        void signalConnect();

        void initUi();

        void initEnableStatus();

        void load();

        void save();

    private slots:

        void handleOK();

        void handleApply();

        void handleClose();

        void handleReset();
    };

}

#endif
