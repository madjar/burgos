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
    explicit FtpWidget(FtpModel* ftpModel, QWidget *parent = 0);
    virtual ~FtpWidget();

protected:
    virtual void changeEvent(QEvent *e);
    void on_lineEdit_returnPressed();
    void on_lineEdit_textEdited(const QString &string);

private:
    FtpModel *model;
    ProxyModel *proxy;

    Ui::FtpWidget *m_ui;
};

#endif // FTPWIDGET_H
