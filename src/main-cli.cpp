#include <QtCore>
#include <QLocale>
#include "cli.h"
#include "messagehandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
    
    MessageHandler::pick();
    qDebug("MessageHandler started");

    qDebug("Loading locale");
    QTranslator translator;
    translator.load(QString("burgos_") + QLocale::system().name());
    app.installTranslator(&translator);
    qDebug("locale is %s", qPrintable(QLocale::system().name()));

    Cli cli;
    cli.start();

    qDebug("Starting app");
    return app.exec();
}
