#ifndef BURGOS_H
#define BURGOS_H

#include <QString>
#include <QCloseEvent>
#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QSystemTrayIcon>
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

    Model *model;

    //Ces slots vont peter quand j'aurais trouvé mieux pour le scan
signals:
    void setProgressBarMaximum(int);
    void setProgressBarValue(int);

public slots:
    void returnPressed();
    void textEdited(const QString &string);
    void scan();

    void closeEvent(QCloseEvent *);

    void hideShow(QSystemTrayIcon::ActivationReason reason = QSystemTrayIcon::Unknown);

protected:
    virtual void changeEvent(QEvent *e);
    void createIcon();
    void createActions();
    void createTrayIcon();

private:
    QIcon *icon;

    QAction *hideShowAction;
    QAction *quitAction;

    QMenu *trayIconMenu;

    QSystemTrayIcon *trayIcon;

    Ui::Burgos *m_ui;
    ProxyModel *proxy;
    PeerModel *peer;
};

#endif // BURGOS_H
