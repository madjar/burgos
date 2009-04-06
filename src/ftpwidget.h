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

protected slots:
    void on_lineEdit_returnPressed();
    void on_lineEdit_textEdited(const QString &string);
    void on_treeView_doubleClicked(const QModelIndex & index);

protected:
    virtual void changeEvent(QEvent *e);

private:
    FtpModel *model;
    ProxyModel *proxy;

    Ui::FtpWidget *m_ui;
};

#endif // FTPWIDGET_H
