#include "ftpwidget.h"
#include "ui_ftpwidget.h"

#include <QHeaderView>

FtpWidget::FtpWidget(FtpModel *ftpModel, QWidget *parent) :
    QWidget(parent),
    model(ftpModel),
    m_ui(new Ui::FtpWidget)
{
    m_ui->setupUi(this);

    proxy = new ProxyModel();
    proxy->setSourceModel(model);
    proxy->setFilterCaseSensitivity(Qt::CaseInsensitive);

    m_ui->treeView->setModel(proxy);
    m_ui->treeView->sortByColumn(0,Qt::AscendingOrder);
    m_ui->treeView->header()->setStretchLastSection(false);
    m_ui->treeView->header()->setResizeMode(1,QHeaderView::ResizeToContents);
    m_ui->treeView->header()->setResizeMode(0,QHeaderView::Stretch);
}

FtpWidget::~FtpWidget()
{
    delete m_ui;
}

void FtpWidget::on_lineEdit_returnPressed()
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

void FtpWidget::on_lineEdit_textEdited(const QString &string)
{
    if (string.isEmpty())
    {
        // Hack tout moche pour résoudre le bug #321871
        proxy->setFilterWildcard(string);
        proxy->setFilterWildcard(string);
    }
}

void FtpWidget::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
