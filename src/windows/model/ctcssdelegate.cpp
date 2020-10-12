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

#include <QLineEdit>

#include "ctcssdelegate.hpp"
#include "helper.hpp"

CtcssDelegate::CtcssDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

QWidget *
CtcssDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const {
    QComboBox *comboBox = createCtcssComboBox(parent);
    return comboBox;
}

void CtcssDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *comboBox = dynamic_cast<QComboBox *>(editor);
    QString stringValue = index.model()->data(index).toString();

    for (int i = 0; i < ctcssValues.size(); i++)
        if (stringValue == ctcssValues[i]) {
            comboBox->setCurrentIndex(i);
            return;
        }

    comboBox->setCurrentIndex(0);
}

void CtcssDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    auto comboBox = dynamic_cast<QComboBox *>(editor);
    int data = comboBox->currentData().toInt();
    model->setData(index, data);
}

void CtcssDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const {
    editor->setGeometry(option.rect);
}

QComboBox *CtcssDelegate::createCtcssComboBox(QWidget *parent) {
    auto *comboBox = new QComboBox(parent);

    comboBox->setEditable(true);
    comboBox->lineEdit()->setReadOnly(true);
    comboBox->lineEdit()->setAlignment(Qt::AlignCenter);

    for (int i = 0; i < ctcssValues.size(); i++) {
        comboBox->addItem(ctcssValues[i], i);
        comboBox->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    return comboBox;
}
