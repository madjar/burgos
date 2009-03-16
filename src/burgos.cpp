#include "burgos.h"

#include <QApplication>

#include <QtDebug>
#include <QTabWidget>
#include <QVBoxLayout>
#include "ftpmodel.h"
#include "ftpwidget.h"
#include "logwidget.h"

Burgos::Burgos(QWidget *parent) :
        QWidget(parent)
{
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

    //Ajout des onglets
    QVBoxLayout *layout = new QVBoxLayout(this);
    setLayout(layout);
    QTabWidget *tabWidget = new QTabWidget();
    layout->addWidget(tabWidget);

    FtpModel *model = new FtpModel(this);

    tabWidget->addTab(new FtpWidget(model),tr("&Ftp servers"));
    tabWidget->addTab(new LogWidget(), tr("&Log"));
}

Burgos::~Burgos()
{
    if (icon)
        delete icon;
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
