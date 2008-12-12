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

/* Constructeurs */
private:
    /* Le Singleton */
    static Burgos* singleton_;

protected: //les constructeurs doivent rester proteges
    explicit Burgos(QWidget *parent = 0);
    virtual ~Burgos();

public: //pour un acces a partir des fonctions membres statiques
    static Burgos* pick();
    static void    kill();

/* Signals et Slots */
signals:
    /* Ajoute un element au LogView */
    void appendLog(const QString &);
    void appendLogView(const QString &);

public slots:
    void textEdited(const QString &);
    void appendLogInterface(const QString &);

/* Fonctions membres */
protected:
    /* Message Handler pour la version GUI */
    static void messageHandler(QtMsgType, const char *);

protected:
    virtual void changeEvent(QEvent *e);

/* Membres */
private:
    Ui::Burgos *m_ui;
    Model *model;
    ProxyModel *proxy;
    PeerModel *peer;
};

#endif // BURGOS_H
