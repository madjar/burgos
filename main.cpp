#include <QtCore>
#include <QtGui>
#include <QLocale>
#include "ftp.h"
#include "model.h"
#include "node.h"
#include "proxymodel.h"
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

    //Scanning
    ScanFtp *s = new ScanFtp();
    QObject::connect(s, SIGNAL(maximumChanged(int)),
                   &burgos, SIGNAL(setProgressBarMaximum(int)));
    QObject::connect(s, SIGNAL(progressChanged(int)),
                   &burgos, SIGNAL(setProgressBarValue(int)));
    QObject::connect (s, SIGNAL(found(QString&)),
                    burgos.model, SLOT(addFtp(QString&)));
    s->scan();


    qDebug("Starting app");
    return app.exec();
}
