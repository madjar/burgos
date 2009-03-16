#include "logwidget.h"
#include "ui_logwidget.h"

#include "messagehandler.h"

LogWidget::LogWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::LogWidget)
{
    m_ui->setupUi(this);

    connect(MessageHandler::pick(), SIGNAL(appendLog(const QString &)),
            m_ui->plainTextEdit, SLOT(appendPlainText(const QString &)));
}

LogWidget::~LogWidget()
{
    delete m_ui;
}

void LogWidget::changeEvent(QEvent *e)
{
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
