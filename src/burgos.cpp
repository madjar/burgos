/*

Copyright 2008 by Thomas Duboucher and Georges Dubus.

thomas.duboucher@supelec.fr
georges.dubus@supelec.fr

This software is a computer program whose purpose is to scan and
index ftp servers.

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.

*/

#include "burgos.h"

#include <QApplication>

#include <QtDebug>
#include <QTabWidget>
#include <QVBoxLayout>
#include "ftpmodel.h"
#include "ftpwidget.h"
#include "logwidget.h"
#include "scanwidget.h"

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
    tabWidget->addTab(new ScanWidget(model), "&Scan");
    tabWidget->addTab(new LogWidget(), tr("&Log"));

    resize(sizeHint()); //Needed for the size to ba saved when the widget is hidden
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
        if (!this->isVisible())
        {
            this->hideShowAction->setText(tr("&Hide"));
            this->showNormal();
            this->activateWindow();
        }
        else
        {
            this->hideShowAction->setText(tr("&Show"));
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
