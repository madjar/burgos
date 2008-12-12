#include "burgos.h"
#include "ui_burgos.h"
#include "model.h"
#include "node.h"
#include "ftp.h"
#include "proxymodel.h"
#include "scanftp.h"
#include "messagehandler.h"

#include <QHeaderView>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>

#include <QtDebug>

Burgos::Burgos(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::Burgos)
{
    m_ui->setupUi(this);

    model = new Model();
    QString temp = "jorge";
    model->addFtp(temp);

    proxy = new ProxyModel();
    proxy->setSourceModel(model);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);


    m_ui->treeView->setModel(proxy);
    m_ui->treeView->setSortingEnabled(true);
    m_ui->treeView->sortByColumn(0,Qt::AscendingOrder);
    m_ui->treeView->header()->setStretchLastSection(false);
    m_ui->treeView->header()->setResizeMode(1,QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setResizeMode(0,QHeaderView::Stretch);
    m_ui->treeView->setAnimated(true);


    connect(m_ui->lineEdit,SIGNAL(textEdited(const QString &)),
            this,SLOT(textEdited(const QString &)));

    //Scanning
    ScanFtp *s = new ScanFtp();
    connect(s, SIGNAL(maximumChanged(int)),
            m_ui->progressBar, SLOT(setMaximum(int)));
    connect(s, SIGNAL(progressChanged(int)),
            m_ui->progressBar, SLOT(setValue(int)));
    connect (s, SIGNAL(found(QString&)),
             model, SLOT(addFtp(QString&)));
    s->scan();

    //affichage des pairs
    this->peer = new PeerModel();
    m_ui->treeView1->setModel(peer); //Faudra penser a nommer un peu mieux tout ca ;)
    m_ui->treeView1->setSortingEnabled(true);
    m_ui->treeView1->setItemsExpandable(false);
    m_ui->treeView1->setRootIsDecorated(false);

    connect(peer, SIGNAL(changed(QModelIndex)),
            m_ui->treeView1, SLOT(update(QModelIndex)));


    connect(messageHandler::pick(), SIGNAL(appendLog(const QString &)),
            m_ui->plainTextEdit, SLOT(appendPlainText(const QString &)));
}

Burgos::~Burgos()
{
    delete m_ui;
}

void Burgos::textEdited(const QString &string)
{
    if (string.size() >= 2)
    {
        proxy->setFilterWildcard(string);
        m_ui->treeView->expandAll();
    }
    else
    {
        m_ui->treeView->collapseAll();
        proxy->setFilterWildcard(string);
    }
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
