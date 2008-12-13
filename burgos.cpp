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

    connect(MessageHandler::pick(), SIGNAL(appendLog(const QString &)),
            m_ui->plainTextEdit, SLOT(appendPlainText(const QString &)));

    this->createIcon();

    this->setWindowIcon(*this->icon);
    this->setWindowTitle("Burgos");

    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        this->createActions();
        this->createTrayIcon();
        this->trayIcon->show();
        connect(this->trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(fakeOpen(QSystemTrayIcon::ActivationReason)));
        if (QSystemTrayIcon::supportsMessages())
        this->trayIcon->showMessage("Burgos est lancé",
                                "Maintenant, on est au courant grâce a cette ennuyeux petit tray ;)",
                                QSystemTrayIcon::Information, 2000);
    }

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

void Burgos::closeEvent(QCloseEvent *event)
{
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        this->fakeClose();
        if (QSystemTrayIcon::supportsMessages())
        this->trayIcon->showMessage("Burgos est maintenant réduit",
                                "Burgos continue de tourner en tâche de fond pour actualiser la liste des ftp",
                                QSystemTrayIcon::Information, 2000);
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void Burgos::fakeClose()
{
    this->trayIcon->setContextMenu(this->trayIconMenu);
    this->showMinimized();
    this->hide();
}

void Burgos::fakeOpen()
{
    this->trayIcon->setContextMenu(this->trayMiniIconMenu);
    this->show();
    this->showNormal();
    this->activateWindow();
}

void Burgos::fakeOpen(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick ||
        reason == QSystemTrayIcon::Trigger ||
        reason == QSystemTrayIcon::MiddleClick)
    {
        this->trayIcon->setContextMenu(this->trayMiniIconMenu);
        this->show();
        this->showNormal();
    }
}

void Burgos::changeEvent(QEvent *event)
{
    switch(event->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void Burgos::createIcon()
{
    this->icon = new QIcon(QPixmap(":/icons/computer.png"));
}

void Burgos::createActions()
{
    this->restoreAction = new QAction(tr("&Ouvrir"), this);
    connect(this->restoreAction, SIGNAL(triggered()), this, SLOT(fakeOpen()));
    this->quitAction = new QAction(tr("&Quitter"), this);
    connect(this->quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Burgos::createTrayIcon()
{
    this->trayIconMenu = new QMenu(this);
    this->trayIconMenu->addAction(this->restoreAction);
    //this->trayIconMenu->addSeparator();
    this->trayIconMenu->addAction(this->quitAction);
    this->trayMiniIconMenu = new QMenu(this);
    this->trayMiniIconMenu->addAction(this->quitAction);
    this->trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setContextMenu(this->trayMiniIconMenu);
    this->trayIcon->setIcon(QIcon(*this->icon));
}
