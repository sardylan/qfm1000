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


#ifndef __QFM1000__QFM1000__INSTANCE__H
#define __QFM1000__QFM1000__INSTANCE__H

#include <QtCore/QtGlobal>
#include <QtCore/QObject>
#include <QtCore/QByteArray>
#include <QtCore/QString>

#include <eeprom/eeprom.hpp>

#include "windows/instance.hpp"

using namespace qfm1000;

namespace qfm1000::app {

    class Instance : public QObject {
    Q_OBJECT

    public:

        explicit Instance(quint64 id, QObject *parent = nullptr);

        ~Instance() override;

        [[nodiscard]] quint64 getId() const;

        [[nodiscard]] eeprom::EEPROM *getEeprom() const;

        [[nodiscard]] eeprom::EEPROM *getOldEeprom() const;

        [[nodiscard]] windows::Instance *getWindow() const;

        [[nodiscard]] const QString &getFileName() const;

        void setFileName(const QString &newValue);

    public slots:

        void resetStatus();

    private:

        const quint64 id;
        QString fileName;

        eeprom::EEPROM *eeprom;
        eeprom::EEPROM *oldEeprom;

        windows::Instance *window;


    };

}

#endif
