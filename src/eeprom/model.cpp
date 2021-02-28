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


#include <QtCore/QtMath>

#include "model.hpp"
#include "values.hpp"

using namespace qfm1000::eeprom;

TableModel::TableModel(EEPROM *eeprom, QObject *parent) : QAbstractTableModel(parent) {
    TableModel::eeprom = eeprom;
}

TableModel::~TableModel() = default;

EEPROM *TableModel::getEeprom() const {
    return eeprom;
}

int TableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return CHANNELS_COUNT;
}

int TableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return QFM1000_TABLE_MODEL_COLUMNS;
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
    if (!index.isValid())
        return QVariant();

    if (index.row() >= CHANNELS_COUNT || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        int channel = index.row();

        switch (index.column()) {
            case 0:
                return Values::frequency(eeprom->getChannelRxFreq(channel));
            case 1:
                return Values::frequency(eeprom->getChannelTxFreq(channel));
            case 2:
                return Values::shift(eeprom->getChannelTxFreq(channel), eeprom->getChannelRxFreq(channel));
            case 3:
                return Values::ctcss(eeprom->getChannelRxCtcss(channel));
            case 4:
                return Values::ctcss(eeprom->getChannelTxCtcss(channel));
            case 5:
                return Values::power(eeprom->getChannelPower(channel));
            case 6:
                return Values::squelch(eeprom->getChannelSquelch(channel));
            case 7:
                return Values::flag(eeprom->getChannelSelectiveCalling(channel));
            case 8:
                return Values::flag(eeprom->getChannelCpuOffset(channel));
            default:
                return QVariant();
        }
    } else if (role == Qt::TextAlignmentRole) {
        return Qt::AlignCenter;
    }

    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        switch (section) {
            case 0:
                return tr("RX Freq (MHz)");
            case 1:
                return tr("TX Freq (MHz)");
            case 2:
                return tr("Shift (kHz)");
            case 3:
                return tr("RX CTCSS");
            case 4:
                return tr("TX CTCSS");
            case 5:
                return tr("Power");
            case 6:
                return tr("Squelch");
            case 7:
                return tr("Selective Calling");
            case 8:
                return tr("CPU Offset");
            default:
                return QVariant();
        }
    } else if (orientation == Qt::Vertical) {
        return section;
    }

    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        Channel channel = index.row();
        Frequency newFrequency;

        switch (index.column()) {
            case 0:
                newFrequency = Values::parseFrequencyString(value.toString());
                if (newFrequency == 0)
                    return false;
                eeprom->setChannelRxFreq(channel, newFrequency);
                break;

            case 1:
                newFrequency = Values::parseFrequencyString(value.toString());
                if (newFrequency == 0)
                    return false;
                eeprom->setChannelTxFreq(channel, newFrequency);
                break;

            case 3:
                eeprom->setChannelRxCtcss(channel, value.value<CTCSS>());
                break;

            case 4:
                eeprom->setChannelTxCtcss(channel, value.value<CTCSS>());
                break;

            case 5:
                eeprom->setChannelPower(channel, value.value<Power>());
                break;

            case 6:
                eeprom->setChannelSquelch(channel, value.value<Squelch>());
                break;

            case 7:
                eeprom->setChannelSelectiveCalling(channel, value.value<Flag>());
                break;

            case 8:
                eeprom->setChannelCpuOffset(channel, value.value<Flag>());
                break;

            default:
                return false;
        }

        emit(dataChanged(index, index));
        return true;
    }

    return false;
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value) {
    return setData(index, value, Qt::EditRole);
}

Qt::ItemFlags TableModel::flags(const QModelIndex &index) const {
    if (!index.isValid())
        return Qt::ItemIsEnabled;

    Qt::ItemFlags itemFlags = QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    return itemFlags;
}

int TableModel::paramToColumn(Param param) {
    switch (param) {
        case Param::PARAM_FREQ_RX:
            return 0;
        case Param::PARAM_FREQ_TX:
            return 1;
        case Param::PARAM_CTCSS_RX:
            return 3;
        case Param::PARAM_CTCSS_TX:
            return 4;
        case Param::PARAM_POWER:
            return 5;
        case Param::PARAM_SQUELCH:
            return 6;
        case Param::PARAM_SELECTIVE_CALLING:
            return 7;
        case Param::PARAM_CPU_OFFSET:
            return 8;
        default:
            return -1;
    }
}

Param TableModel::columnToParam(int column) {
    switch (column) {
        case 0:
            return Param::PARAM_FREQ_RX;
        case 1:
            return Param::PARAM_FREQ_TX;
        case 3:
            return Param::PARAM_CTCSS_RX;
        case 4:
            return Param::PARAM_CTCSS_TX;
        case 5:
            return Param::PARAM_POWER;
        case 6:
            return Param::PARAM_SQUELCH;
        case 7:
            return Param::PARAM_SELECTIVE_CALLING;
        case 8:
            return Param::PARAM_CPU_OFFSET;
        default:
            return Param::NONE;
    }
}
