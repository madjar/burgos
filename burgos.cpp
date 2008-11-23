#include "burgos.h"
#include "ui_burgos.h"
#include "model.h"
#include "node.h"
#include "ftp.h"
#include "proxymodel.h"

#include <QHeaderView>
#include <QtDebug>

Burgos::Burgos(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::Burgos)
{
    m_ui->setupUi(this);

    model = new Model();
    model->addFtp("jorge");

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
