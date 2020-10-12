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

#ifndef __QFM1000_WINDOWS_MODEL_TABLEMODEL_H
#define __QFM1000_WINDOWS_MODEL_TABLEMODEL_H

#include <QAbstractTableModel>
#include <QPair>
#include <QList>
#include <QString>

#include <status.hpp>
#include <eeprom.hpp>

class TableModel : public QAbstractTableModel {
Q_OBJECT

private:
    Status *status;
    EEPROM *eeprom;

    static unsigned int strFreqToInt(QString input);

    static QString intFreqToStr(unsigned int input);

    static QString shiftToStr(unsigned int txFreq, unsigned int rxFreq);

    static QString boolToStr(bool value);

public:
    explicit TableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent) const override;

    int columnCount(const QModelIndex &parent) const override;

    QVariant data(const QModelIndex &index, int role) const override;

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex &index) const override;

    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

};

#endif
