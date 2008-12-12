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
    void appendLogInterface(const QString &);

signals:
    /* Ajoute un element au LogView */
    void appendLog(const QString &);
    void appendLogView(const QString &);

protected:
    /* Message Handler pour la version GUI */
    static void messageHandler(QtMsgType, const char *);

    virtual void changeEvent(QEvent *e);

private:
    Ui::Burgos *m_ui;
    Model *model;
    ProxyModel *proxy;
    PeerModel *peer;
};

#endif // BURGOS_H
