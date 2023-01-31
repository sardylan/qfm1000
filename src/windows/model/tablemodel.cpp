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

#include <QRegExp>
#include <QtMath>

#include "tablemodel.hpp"
#include "helper.hpp"
#include "utilities.hpp"

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent) {
    eeprom = EEPROM::getInstance();
    status = Status::getInstance();
}

int TableModel::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return CHANNELS_COUNT;
}

int TableModel::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 9;
}

QVariant TableModel::data(const QModelIndex &index, int role) const {
    unsigned int value;

    if (!index.isValid())
        return QVariant();

    if (index.row() >= CHANNELS_COUNT || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        int channel = index.row();

        switch (index.column()) {
            case 0:
                return ModelUtilities::intFreqToStr(eeprom->getChannelRxFreq(channel, status->getFrequencyBand()));
            case 1:
                return ModelUtilities::intFreqToStr(eeprom->getChannelTxFreq(channel, status->getFrequencyBand()));
            case 2: {
                unsigned int txFreq = eeprom->getChannelTxFreq(channel, status->getFrequencyBand());
                unsigned int rxFreq = eeprom->getChannelRxFreq(channel, status->getFrequencyBand());
                return ModelUtilities::shiftToStr(txFreq, rxFreq);
            }
            case 3: {
                value = eeprom->getChannelRxCtcss(channel);
                if (value < 0)
                    value = 0;
                if (value > ctcssValues.size() - 1)
                    value = ctcssValues.size() - 1;
                return ctcssValues[value];
            }
            case 4: {
                value = eeprom->getChannelTxCtcss(channel);
                if (value < 0)
                    value = 0;
                if (value > ctcssValues.size() - 1)
                    value = ctcssValues.size() - 1;
                return ctcssValues[value];
            }
            case 5: {
                value = eeprom->getChannelSquelch(channel);
                if (value < 0)
                    value = 0;
                if (value > squelchValues.size() - 1)
                    value = squelchValues.size() - 1;
                return squelchValues[value];
            }
            case 6: {
                value = eeprom->getChannelPower(channel);
                if (value < 0)
                    value = 0;
                if (value > powerValues.size() - 1)
                    value = powerValues.size() - 1;
                return powerValues[value];
            }
            case 7:
                return ModelUtilities::boolToStr(eeprom->getChannelSelectiveCalling(channel));
            case 8:
                return ModelUtilities::boolToStr(eeprom->getChannelCpuOffset(channel));
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
                return tr("RX Freq (kHz)");
            case 1:
                return tr("TX Freq (kHz)");
            case 2:
                return tr("Shift (kHz)");
            case 3:
                return tr("RX CTCSS");
            case 4:
                return tr("TX CTCSS");
            case 5:
                return tr("Squelch");
            case 6:
                return tr("Power");
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
        int channel = index.row();
        unsigned int newValue = 0;
        switch (index.column()) {
            case 0:
                newValue = ModelUtilities::strFreqToInt(value.toString());
                if (newValue == 0)
                    return false;
                eeprom->setChannelRxFreq(channel, newValue, status->getFrequencyBand());
                break;

            case 1:
                newValue = ModelUtilities::strFreqToInt(value.toString());
                if (newValue == 0)
                    return false;
                eeprom->setChannelTxFreq(channel, newValue, status->getFrequencyBand());
                break;

            case 3:
                eeprom->setChannelRxCtcss(channel, value.toUInt());
                break;

            case 4:
                eeprom->setChannelTxCtcss(channel, value.toUInt());
                break;

            case 5:
                eeprom->setChannelSquelch(channel, value.toUInt());
                break;

            case 6:
                eeprom->setChannelPower(channel, value.toUInt());
                break;

            case 7:
                eeprom->setChannelSelectiveCalling(channel, value.toBool());
                break;

            case 8:
                eeprom->setChannelCpuOffset(channel, value.toBool());
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

    Qt::ItemFlags itemFlags = QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
    return itemFlags;
}
