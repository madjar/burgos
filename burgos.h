#ifndef BURGOS_H
#define BURGOS_H

#include <QtGui/QDialog>
#include <QString>
#include "model.h"
#include "proxymodel.h"

namespace Ui {
    class Burgos;
}

class Burgos : public QDialog {
    Q_OBJECT
    Q_DISABLE_COPY(Burgos)
public:
    explicit Burgos(QWidget *parent = 0);
    virtual ~Burgos();

public slots:
    void textEdited(const QString &string);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::Burgos *m_ui;
    Model *model;
    ProxyModel *proxy;
};

#endif // BURGOS_H
