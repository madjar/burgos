#ifndef BURGOS_H
#define BURGOS_H

#include <QtGui/QWidget>
#include <QString>
#include "model.h"
#include "proxymodel.h"
#include "peermodel.h"

namespace Ui {
    class Burgos;
}

class Burgos : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(Burgos)

public:
    explicit Burgos(QWidget *parent = 0);
    virtual ~Burgos();

public slots:
    void textEdited(const QString &);

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::Burgos *m_ui;
    Model *model;
    ProxyModel *proxy;
    PeerModel *peer;
};

#endif // BURGOS_H
