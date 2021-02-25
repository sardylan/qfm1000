#include <QtCore/QDebug>
#include <QtCore/QByteArray>

#include <QtGui/QFontDatabase>

#include <QtWidgets/QApplication>
#include <QtWidgets/QScrollArea>

#include "../hexeditor.hpp"

int main(int argc, char **argv) {
    QApplication application(argc, argv);

    qfm1000::widgets::HexEditor hexEditor;

    QScrollArea scrollArea;
    scrollArea.setWidget(&hexEditor);
    scrollArea.setWidgetResizable(true);
    scrollArea.show();

    QByteArray data = QByteArray(
            "\x00\x01\x02\x03\x04\xa6\x3d\xa2"
            "\x34\xb2\x1e\x48\xeb\x26\x78\x3c"
            "\x68\x6a\xa6\xb2\xf2\xb9\x8b\xf2"
            "\x31\x7a\x1b\xd2\x56\x6b\xc1\x4f"
            "\x2e\x14\x3e\x2b\xeb\x46\xa7\x45"
            "\xd2\x1c\x3c\x0a\xb1\xbe\x1b\x33"
            "\xa7\xd5\x70\x98\x94\x7c\xf6\x22",
            56
    );

    QMetaObject::invokeMethod(&hexEditor, "setData", Qt::QueuedConnection, Q_ARG(QByteArray, data));

    return QApplication::exec();
}
