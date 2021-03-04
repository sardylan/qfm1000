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


#include <QtCore/QSize>

#include <QtWidgets/QLineEdit>

#include <eeprom/eeprom.hpp>
#include <eeprom/values.hpp>

#include "ctcss.hpp"

using namespace qfm1000::delegators;

CTCSSDelegate::CTCSSDelegate(QObject *parent) : QStyledItemDelegate(parent) {
}

CTCSSDelegate::~CTCSSDelegate() = default;

QWidget *CTCSSDelegate::createEditor(QWidget *parent,
                                     const QStyleOptionViewItem &option,
                                     const QModelIndex &index) const {
    Q_UNUSED(option)
    Q_UNUSED(index)

    QComboBox *comboBox = createCtcssComboBox(parent);
    return comboBox;
}

void CTCSSDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const {
    auto *comboBox = dynamic_cast<QComboBox *>(editor);
    auto value = index.model()->data(index, Qt::UserRole).value<eeprom::CTCSS>();

    for (int i = 0; i <= 38; i++) {
        if (static_cast<eeprom::CTCSS>(i) == value) {
            comboBox->setCurrentIndex(i);
            return;
        }
    }

    comboBox->setCurrentIndex(0);
}

void CTCSSDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const {
    auto comboBox = dynamic_cast<QComboBox *>(editor);
    auto data = comboBox->currentData();
    model->setData(index, data);
}

void CTCSSDelegate::updateEditorGeometry(QWidget *editor,
                                         const QStyleOptionViewItem &option,
                                         const QModelIndex &index) const {
    Q_UNUSED(index)

    editor->setGeometry(option.rect);
}

QComboBox *CTCSSDelegate::createCtcssComboBox(QWidget *parent) {
    auto *comboBox = new QComboBox(parent);

    comboBox->setEditable(true);
    comboBox->lineEdit()->setReadOnly(true);
    comboBox->lineEdit()->setAlignment(Qt::AlignCenter);

    for (int i = 0; i <= 38; i++) {
        auto value = static_cast<eeprom::CTCSS>(i);
        comboBox->addItem(eeprom::Values::ctcss(value), QVariant::fromValue(value));
        comboBox->setItemData(i, Qt::AlignCenter, Qt::TextAlignmentRole);
    }

    return comboBox;
}

QSize CTCSSDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const {
    const QSize &size = QStyledItemDelegate::sizeHint(option, index);
    return QSize(size.width() + 55, size.height());
}
