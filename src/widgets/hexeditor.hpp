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


#ifndef __QFM1000__WIDGETS__HEXEDITOR__H
#define __QFM1000__WIDGETS__HEXEDITOR__H

#include <QtCore/QByteArray>
#include <QtCore/QMap>

#include <QtWidgets/QWidget>

#define HEXEDITOR_PAGESIZE_DEFAULT 8

#define HEXEDITOR_MARGIN_TOP_DEFAULT 10
#define HEXEDITOR_MARGIN_LEFT_DEFAULT 2

#define HEXEDITOR_CELL_SIZE_VERTICAL_DEFAULT 16
#define HEXEDITOR_CELL_SIZE_HORIZONTAL_DEFAULT 20

#define HEXEDITOR_FONT_SIZE_DEFAULT 10

namespace qfm1000::widgets {

    class HexEditor : public QWidget {
    Q_OBJECT

    public:

        explicit HexEditor(QWidget *parent = nullptr);

        ~HexEditor() override;

        [[nodiscard]] const QByteArray &getData() const;

        [[nodiscard]] int getPageSize() const;

    public slots:

        void setPageSize(int newValue);

        void setData(const QByteArray &newValue);

        void setByte(int pos, const char &byte);

        void setByteSelected(int bytePosition, bool selected);

    protected:

        void paintEvent(QPaintEvent *event) override;

    private:

        QByteArray data;
        int pageSize;

        QMap<int, bool> selectionMap;

        int marginTop;
        int marginLeft;

        int cellSizeVertical;
        int cellSizeHorizontal;

        int fontSize;

        int rows;
        int addressesSize;
        int marginHex;

        void initSelectionMap();

        void updateRows();

        void updatePosSize();

        void invokeUpdate();

        void paintPageAddresses(QPainter &painter, int page, int offset) const;

        void paintPageData(QPainter &painter, int page, int offset);

    signals:

        void ensureVisible(int x, int y);

    };

}

#endif
