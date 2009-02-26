#include "burgos.h"
#include "ui_burgos.h"
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
    this->setWindowTitle(tr("Burgos"));

    //Configuration du tray
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        this->createActions();
        this->createTrayIcon();
        this->trayIcon->show();
        connect(this->trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
                this, SLOT(hideShow(QSystemTrayIcon::ActivationReason)));
        if (QSystemTrayIcon::supportsMessages())
            this->trayIcon->showMessage(tr("Burgos is running"),
                                        tr("Anyway, this message is going to disapear sonner or latter."),
                                        QSystemTrayIcon::Information, 2000);
    }

    model = new FtpModel();

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


    connect(m_ui->lineEdit,SIGNAL(returnPressed()),
            this,SLOT(returnPressed()));
    connect(m_ui->lineEdit,SIGNAL(textEdited(const QString &)),
            this,SLOT(textEdited(const QString &)));

    //affichage des pairs
    this->peer = new PeerModel();
    m_ui->peerView->setModel(peer);
    m_ui->peerView->setSortingEnabled(false);
    m_ui->peerView->setItemsExpandable(false);
    m_ui->peerView->setRootIsDecorated(false);

    connect(peer, SIGNAL(changed(QModelIndex)),
            m_ui->peerView, SLOT(update(QModelIndex)));

    //Config de la ProgressBar
    connect(this, SIGNAL(setProgressBarMaximum(int)),
            m_ui->progressBar, SLOT(setMaximum(int)));
    connect(this, SIGNAL(setProgressBarValue(int)),
            m_ui->progressBar, SLOT(setValue(int)));
    //Et du bouton
    connect(m_ui->scanButton, SIGNAL(clicked()),
            this, SLOT(scan()));
}

Burgos::~Burgos()
{
    delete m_ui;
    if (icon)
        delete icon;
}

void Burgos::returnPressed()
{
    QString text = m_ui->lineEdit->text();
    if (text.startsWith("addftp:"))
    {
        model->addFtp(text.split(':').at(1));
        m_ui->lineEdit->clear();
    }
    else if (!text.isEmpty())
    {
        proxy->setFilterWildcard(text);
        m_ui->treeView->expandAll();
    }
}

void Burgos::textEdited(const QString &string)
{
    if (string.isEmpty())
        proxy->setFilterWildcard(string);
}

void Burgos::scan()
{
    m_ui->scanButton->setEnabled(false);
    ScanFtp *s = new ScanFtp();
    QObject::connect(s, SIGNAL(maximumChanged(int)),
                   this, SIGNAL(setProgressBarMaximum(int)));
    QObject::connect(s, SIGNAL(progressChanged(int)),
                   this, SIGNAL(setProgressBarValue(int)));
    QObject::connect (s, SIGNAL(found(const QString&)),
                    this->model, SLOT(addFtp(const QString&)));
    s->scan();
}

void Burgos::closeEvent(QCloseEvent *event)
{
    if (QSystemTrayIcon::isSystemTrayAvailable())
    {
        this->hideShow();
        if (QSystemTrayIcon::supportsMessages())
            this->trayIcon->showMessage(tr("Burgos is now reduced"),
                                        tr("Burgos is still running and refreshing the list of ftps"),
                                        QSystemTrayIcon::Information, 2000);
        event->ignore();
    }
    else
    {
        event->accept();
    }
}

void Burgos::hideShow(QSystemTrayIcon::ActivationReason reason)
{
    if (reason == QSystemTrayIcon::DoubleClick ||
        reason == QSystemTrayIcon::Trigger ||
        reason == QSystemTrayIcon::MiddleClick ||
        reason == QSystemTrayIcon::Unknown)
    {
        if (this->isMinimized())
        {
            this->hideShowAction->setText(tr("&Hide"));
            this->show();
            this->showNormal();
            this->activateWindow();
        }
        else
        {
            this->hideShowAction->setText(tr("&Show"));
            this->showMinimized();
            this->hide();
        }
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
    this->icon = new QIcon(":/icons/computer.png");
}

void Burgos::createActions()
{
    this->hideShowAction = new QAction(tr("&Hide"), this);
    connect(this->hideShowAction, SIGNAL(triggered()), this, SLOT(hideShow()));
    this->quitAction = new QAction(tr("&Quit"), this);
    connect(this->quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void Burgos::createTrayIcon()
{
    this->trayIconMenu = new QMenu(this);
    this->trayIconMenu->addAction(this->hideShowAction);
    this->trayIconMenu->addAction(this->quitAction);
    this->trayIcon = new QSystemTrayIcon(this);
    this->trayIcon->setContextMenu(this->trayIconMenu);
    this->trayIcon->setIcon(QIcon(*this->icon));
}