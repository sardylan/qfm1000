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

#include "ctcssdelegate.hpp"
#include "ctcss.hpp"

CtcssDelegate::CtcssDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

QWidget *
CtcssDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QComboBox *comboBox = createCtcssComboBox(parent);
    return comboBox;
}

void CtcssDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox *>(editor);
    int value = index.model()->data(index).toInt();
    int comboIndex = comboBox->findData(value);
    if (comboIndex != -1)
        comboBox->setCurrentIndex(value);
    else
        comboBox->setCurrentIndex(0);
}

void CtcssDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    QComboBox *comboBox = static_cast<QComboBox *>(editor);
    int data = comboBox->currentData().toInt();
    model->setData(index, data);
}

QComboBox *CtcssDelegate::createCtcssComboBox(QWidget *parent) {
    QComboBox *comboBox = new QComboBox(parent);
    for (int i = 0; i < ctcssValues.size(); i++)
        comboBox->addItem(ctcssValues[i], i);
    return comboBox;
}
