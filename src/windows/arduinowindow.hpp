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

#ifndef __QFM1000_WINDOWS_ARDUINOWINDOW_H
#define __QFM1000_WINDOWS_ARDUINOWINDOW_H

#include <QDialog>

namespace Ui {
    class ArduinoWindow;
}

namespace qfm1000::windows {

    class ArduinoWindow : public QDialog {
    Q_OBJECT

    public:
        explicit ArduinoWindow(QWidget *parent = nullptr);

        ~ArduinoWindow() override;

    public slots:

        void start();

        void finish();

        void log(const QString &text);

        void progress(quint8 value);

    private:
        Ui::ArduinoWindow *ui;

        void signalConnect();

        void initUi();

    };

}

#endif
