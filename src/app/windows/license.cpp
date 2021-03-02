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


#include <QtCore/QDebug>
#include <QtCore/QString>
#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <QtCore/QRect>

#include <QtGui/QPixmap>

#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QDialogButtonBox>

#include "license.hpp"
#include "ui_license.h"

using namespace qfm1000::app::windows;

License::License(QWidget *parent) : QDialog(parent), ui(new Ui::License) {
    ui->setupUi(this);

    connectSignals();
    initUi();
}

License::~License() {
    delete ui;
}

void License::connectSignals() {
    qInfo() << "Connecting signals";

    connect(ui->buttonBox->button(QDialogButtonBox::Close), &QAbstractButton::clicked, this, &License::close);
}

void License::initUi() {
    qInfo() << "Initalizing UI";

    QByteArray data = readFile(":/images/licenses/gpl-v3.svg");
    QPixmap pixmap;
    pixmap.loadFromData(data);
    const QPixmap &scaled = pixmap.scaledToHeight(31, Qt::SmoothTransformation);
    ui->imageLabel->setPixmap(scaled);

#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
    data = readFile(":/files/text/gpl-3.0.md");
    ui->textLabel->setTextFormat(Qt::MarkdownText);
#else
    data = readFile(":/files/text/gpl-3.0.txt");
#endif

    ui->textLabel->setAlignment(Qt::AlignJustify);
    ui->textLabel->setText(QString(data));
    ui->textLabel->setWordWrap(true);
}

QByteArray License::readFile(const QString &path) {
    qInfo() << "Reading file" << path;

    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QByteArray data = file.readAll();
    file.close();
    return data;
}
