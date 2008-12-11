#include "burgos.h"
#include "ui_burgos.h"
#include "model.h"
#include "node.h"
#include "ftp.h"
#include "proxymodel.h"
#include "scanftp.h"

#include <QHeaderView>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>
#include <QHostAddress>

#include <QProgressDialog>

#include <QtDebug>

Burgos* Burgos::singleton_ = NULL;

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


    connect(m_ui->lineEdit,SIGNAL(textEdited(const QString &)),
            this,SLOT(textEdited(const QString &)));

    //Scanning/*
    ScanFtp *s = new ScanFtp();
    connect(s, SIGNAL(maximumChanged(int)),
            m_ui->progressBar, SLOT(setMaximum(int)));
    connect(s, SIGNAL(progressChanged(int)),
            m_ui->progressBar, SLOT(setValue(int)));
    connect (s, SIGNAL(found(QString&)),
             model, SLOT(addFtp(QString&)));
    s->scan();

    //installe le nouvau messageHandler
    qInstallMsgHandler(Burgos::messageHandler);
    connect(this, SIGNAL(appendLog(const QString &)), this, SLOT(appendLogInterface(const QString &)));
    connect(this, SIGNAL(appendLogView(const QString &)), m_ui->plainTextEdit, SLOT(appendPlainText(const QString &)));
    /* aucune fonction qDebug, qWarning, qCritical ou qFatal a partir d'ici */
    emit appendLog("Debug: Message Handler Started");
}

Burgos::~Burgos()
{
    delete m_ui;
}

Burgos* Burgos::pick()
{
    if (singleton_ == NULL) { singleton_ = new Burgos(); }
    return singleton_;
}

void Burgos::kill()
{
    delete singleton_;
    singleton_ = NULL;
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

/*
 * Burgos::appendLogInterface
 * gere le collage des messages dans la fenetre de messages
 * string: message a coller
 */
void Burgos::appendLogInterface(const QString &string)
{
    emit appendLogView(string);
    //fprintf(stdout, string.toStdString().c_str());
}

/*
 * Burgos::messageHandler
 * genere l'affichage des messages sur la fenetre de log
 * est installe quand Burgos est cree
 */
//TODO: reflechir a mettre une interface au signal pour intercepter les messages simples _apres_ le msghandler
void Burgos::messageHandler(QtMsgType type, const char *msg)
{
    switch (type)
    {
    case QtDebugMsg:
        emit Burgos::pick()->appendLogView(QString("Debug: ") + msg);
        break;
    case QtWarningMsg:
        emit Burgos::pick()->appendLogView(QString("Warning: ") + msg);
        break;
    case QtCriticalMsg:
        emit Burgos::pick()->appendLogView(QString("Critical: ") + msg);
        break;
    case QtFatalMsg:
        emit Burgos::pick()->appendLogView(QString("Fatal: ") + msg);
        abort();
    }
    /* Pour la version sans GUI */
/*  switch (type) {
    case QtDebugMsg:
        fprintf(stderr, "Debug: %s\n", msg);
        break;
    case QtWarningMsg:
        fprintf(stderr, "Warning: %s\n", msg);
        break;
    case QtCriticalMsg:
        fprintf(stderr, "Critical: %s\n", msg);
        break;
    case QtFatalMsg:
        fprintf(stderr, "Fatal: %s\n", msg);
        abort();
    }*/
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
