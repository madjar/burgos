#include <QtCore>
#include <QtGui>
#include <QLocale>
#include "burgos.h"
#include "messagehandler.h"
#include "buffererrorhandler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

#ifdef Q_OS_UNIX
    if (QCoreApplication::arguments().contains("--increase-buffer"))
        return BufferErrorHandler::increaseBuffer();
#endif

    MessageHandler::pick();
    qDebug("MessageHandler started");

    qDebug("Loading locale");
    QTranslator translator;
    translator.load(QString("burgos_") + QLocale::system().name());
    app.installTranslator(&translator);
    qDebug("locale is %s", qPrintable(QLocale::system().name()));

    Burgos burgos;
    qDebug("Burgos started");
    burgos.show();

    qDebug("Starting app");
    return app.exec();
}
