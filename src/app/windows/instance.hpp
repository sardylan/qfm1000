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


#ifndef __QFM1000__QFM1000__WINDOWS__EEPROM__H
#define __QFM1000__QFM1000__WINDOWS__EEPROM__H

#include <QtWidgets/QWidget>

namespace Ui {
    class Instance;
}

namespace qfm1000::app::windows {

    class Instance : public QWidget {
    Q_OBJECT

    public:

        explicit Instance(QWidget *parent = nullptr);

        ~Instance() override;

    private:

        Ui::Instance *ui;

        void connectSignals();

        void initUi();

    };

}

#endif
