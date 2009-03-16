#ifndef FTPWIDGET_H
#define FTPWIDGET_H

#include <QtGui/QWidget>
#include "ftpmodel.h"
#include "proxymodel.h"

namespace Ui {
    class FtpWidget;
}

class FtpWidget : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(FtpWidget)
public:
    explicit FtpWidget(QWidget *parent = 0);
    virtual ~FtpWidget();

protected:
    virtual void changeEvent(QEvent *e);
    void on_lineEdit_returnPressed();
    void on_lineEdit_textEdited(const QString &string);

private:
    Ui::FtpWidget *m_ui;

    FtpModel *model;
    ProxyModel *proxy;
};

#endif // FTPWIDGET_H
