#include <QtCore>
#include <QtGui>
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

    Burgos b;
    qDebug("Burgos started");
    b.show();

    //ScanFtp s;
    //s.scan();

    qDebug("Starting app");
    return app.exec();
}
