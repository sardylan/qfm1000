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


#ifndef __QFM1000__DELEGATORS__SQUELCH__H
#define __QFM1000__DELEGATORS__SQUELCH__H

#include <QtCore/QByteArray>
#include <QtCore/QMap>

#include <QtWidgets/QComboBox>
#include <QtWidgets/QStyledItemDelegate>

namespace qfm1000::delegators {

    class SquelchDelegate : public QStyledItemDelegate {
    Q_OBJECT

    public:

        explicit SquelchDelegate(QObject *parent = nullptr);

        ~SquelchDelegate() override;

        QWidget *createEditor(QWidget *parent,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

        void setEditorData(QWidget *editor, const QModelIndex &index) const override;

        void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const override;

        void updateEditorGeometry(QWidget *editor,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index) const override;

        [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    private:

        static QComboBox *createSquelchComboBox(QWidget *parent);
    };

}

#endif
