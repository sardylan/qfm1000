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


#ifndef __QFM1000__EEPROM__MODEL__H
#define __QFM1000__EEPROM__MODEL__H

#include <QtCore/QVariant>
#include <QtCore/QString>
#include <QtCore/QModelIndex>
#include <QtCore/QAbstractTableModel>

#include "eeprom.hpp"

#define QFM1000_TABLE_MODEL_COLUMNS 9

namespace qfm1000::eeprom {

    class TableModel : public QAbstractTableModel {
    Q_OBJECT

    public:

        explicit TableModel(EEPROM *eeprom, QObject *parent = nullptr);

        ~TableModel() override;

        [[nodiscard]] int rowCount(const QModelIndex &parent) const override;

        [[nodiscard]] int columnCount(const QModelIndex &parent) const override;

        [[nodiscard]] QVariant data(const QModelIndex &index, int role) const override;

        [[nodiscard]] QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        [[nodiscard]] Qt::ItemFlags flags(const QModelIndex &index) const override;

        bool setData(const QModelIndex &index, const QVariant &value, int role) override;

        bool setData(const QModelIndex &index, const QVariant &value);

    private:

        EEPROM *eeprom;

        static unsigned int strFreqToInt(QString input);

    };

}

#endif
