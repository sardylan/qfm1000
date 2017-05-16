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

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent) {
    eeprom = EEPROM::getInstance();
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
    if (!index.isValid())
        return QVariant();

    if (index.row() >= CHANNELS_COUNT || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole) {
        Channel *channel = eeprom->getChannel(index.row());

        switch (index.column()) {
            case 0:
                return index.row();
            case 1:
                return intFreqToStr(channel->getRxFreq());
            case 2:
                return intFreqToStr(channel->getTxFreq());
            case 3:
                return shiftToStr(channel->getTxFreq(), channel->getRxFreq());
            case 4:
                return ctcssValues[channel->getRxCtcss()];
            case 5:
                return ctcssValues[channel->getTxCtcss()];
            case 6:
                return powerValues[channel->getPower()];
            case 7:
                return boolToStr(channel->isSelectiveCalling());
            case 8:
                return boolToStr(channel->isCpuOffset());
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
                return tr("Channel");
            case 1:
                return tr("RX Freq (kHz)");
            case 2:
                return tr("TX Freq (kHz)");
            case 3:
                return tr("Shift (kHz)");
            case 4:
                return tr("RX CTCSS");
            case 5:
                return tr("TX CTCSS");
            case 6:
                return tr("Power");
            case 7:
                return tr("Selective Calling");
            case 8:
                return tr("CPU Offset");
            default:
                return QVariant();
        }
    }

    return QVariant();
}

bool TableModel::setData(const QModelIndex &index, const QVariant &value, int role) {
    if (index.isValid() && role == Qt::EditRole) {
        int row = index.row();

        Channel *channel = eeprom->getChannel(row);
        unsigned int newValue = 0;
        switch (index.column()) {
            case 0:
                return false;

            case 1:
                newValue = strFreqToInt(value.toString());
                if (newValue == 0)
                    return false;
                channel->setRxFreq(newValue);
                break;

            case 2:
                newValue = strFreqToInt(value.toString());
                if (newValue == 0)
                    return false;
                channel->setTxFreq(newValue);
                break;

            case 3:
                return false;

            case 4:
                channel->setRxCtcss(value.toUInt());
                break;

            case 5:
                channel->setTxCtcss(value.toUInt());
                break;

            case 6:
                channel->setPower(value.toUInt());
                break;

            case 7:
                channel->setSelectiveCalling(value.toBool());
                break;

            case 8:
                channel->setCpuOffset(value.toBool());
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

unsigned int TableModel::strFreqToInt(QString input) {
    if (input.contains(QRegExp("^\\d{3}\\.{0,1}$"))) {
        QRegExp regExp("^(\\d{3})\\.{0,1}$");
        regExp.indexIn(input);
        return regExp.cap(1).leftJustified(3, '0').toUInt() * 1000000;
    } else if (input.contains(QRegExp("^\\d{3}\\.{0,1}\\d{0,3}$"))) {
        QRegExp regExp("^(\\d{3})\\.{0,1}(\\d{0,3})$");
        regExp.indexIn(input);
        return regExp.cap(1).toUInt() * 1000000
               + regExp.cap(2).leftJustified(3, '0').toUInt() * 1000;
    } else if (input.contains(QRegExp("^\\d{3}\\.{0,1}\\d{0,3}\\.{0,1}\\d{0,3}$"))) {
        QRegExp regExp("^(\\d{3})\\.{0,1}(\\d{0,3})\\.{0,1}(\\d{0,3})$");
        regExp.indexIn(input);
        return regExp.cap(1).toUInt() * 1000000
               + regExp.cap(2).leftJustified(3, '0').toUInt() * 1000
               + regExp.cap(3).leftJustified(3, '0').toUInt();
    } else {
        return 0;
    }
}

QString TableModel::intFreqToStr(unsigned int input) {
    QString value = QString::number(input);
    value.insert(3, '.');
    value.insert(7, ',');
    value.chop(2);
    return value;
}

QString TableModel::shiftToStr(unsigned int txFreq, unsigned int rxFreq) {
    int shift = (int) qFabs((int) txFreq - (int) rxFreq);
    QString value = QString::number(shift);
    value.insert(3, '.');
    value.chop(2);

    if (txFreq > rxFreq)
        value.insert(0, "+");
    else if (txFreq < rxFreq)
        value.insert(0, "-");

    return value;
}

QString TableModel::boolToStr(bool value) {
    return value ? "enabled" : "disabled";
}