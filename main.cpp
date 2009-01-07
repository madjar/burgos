#include <QtCore>
#include <QtGui>
#include <QLocale>
#include "burgos.h"
#include "messagehandler.h"

#include "scanftp.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

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
