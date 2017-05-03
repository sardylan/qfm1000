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

#include "tablemodel.hpp"

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent) {
    eeprom = EEPROM::getInstance();
}

int TableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return CHANNELS_COUNT;
}

int TableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 6;
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= CHANNELS_COUNT || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        Channel *channel = eeprom->channels[index.row()];

        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return channel->getRxFreq();
            case 2:
                return channel->getTxFreq();
            case 3:
                return (int) (channel->getTxFreq() - channel->getRxFreq());
            case 4:
                return channel->getRxCtcss();
            case 5:
                return channel->getTxCtcss();
            default:
                return QVariant();
        }
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("Channel");
            case 1:
                return tr("RX Freq");
            case 2:
                return tr("TX Freq");
            case 3:
                return tr("Shift");
            case 4:
                return tr("RX CTCSS");
            case 5:
                return tr("TX CTCSS");
            default:
                return QVariant();
        }
    }

    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        Channel *channel = eeprom->channels[row];

        switch (index.column()) {
            case 0:
                return false;

            case 1:
                channel->setRxFreq(value.toUInt());
                break;

            case 2:
                channel->setTxFreq(value.toUInt());
                break;

            case 3:
                return false;

            case 4:
                channel->setRxCtcss(value.toUInt());
                break;

            case 5:
                channel->setTxCtcss(value.toUInt());
                break;

            default:
                return false;
        }

        emit(dataChanged(index, index));
        return true;
    }

    return false;
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}
