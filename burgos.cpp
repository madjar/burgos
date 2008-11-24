#include "burgos.h"
#include "ui_burgos.h"
#include "model.h"
#include "node.h"
#include "ftp.h"
#include "proxymodel.h"

#include <QHeaderView>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>

#include <QtDebug>

Burgos::Burgos(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Burgos)
{
    m_ui->setupUi(this);

    model = new Model();
    //model->addFtp("jorge");
    // On ajoute chaque ftp du réseau
    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces())
        foreach (QNetworkAddressEntry entry, iface.addressEntries())
            if(!entry.broadcast().isNull() && entry.ip()!=QHostAddress(QHostAddress::LocalHost))
            {
                quint32 ip= entry.ip().toIPv4Address();
                quint32 netmask = entry.netmask().toIPv4Address();
                quint32 base = ip & netmask;
                for (quint32 current = 0x00000000; current <= (~netmask); current++)
                    model->addFtp( QHostAddress(base + current).toString());
            }

    proxy = new ProxyModel();
    proxy->setSourceModel(model);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);


    m_ui->treeView->setModel(proxy);
    m_ui->treeView->setSortingEnabled(true);
    m_ui->treeView->sortByColumn(0,Qt::AscendingOrder);
    m_ui->treeView->header()->setStretchLastSection(false);
    m_ui->treeView->header()->setResizeMode(1,QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setResizeMode(0,QHeaderView::Stretch);



    connect(m_ui->lineEdit, SIGNAL(textEdited(const QString &)),
            proxy, SLOT(setFilterWildcard(const QString &)));

}

Burgos::~Burgos()
{
    delete m_ui;
}

void Burgos::changeEvent(QEvent *e)
{
    switch(e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
