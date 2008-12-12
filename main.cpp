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
    QApplication a(argc, argv);

    messageHandler::pick();

    Burgos b;
    b.show();

    //ScanFtp s;
    //s.scan();

    return a.exec();
}
