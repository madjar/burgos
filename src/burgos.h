#ifndef BURGOS_H
#define BURGOS_H

#include <QCloseEvent>
#include <QtGui/QWidget>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QSystemTrayIcon>

class Burgos : public QWidget {
    Q_OBJECT
    Q_DISABLE_COPY(Burgos)

public:
    explicit Burgos(QWidget *parent = 0);
    virtual ~Burgos();

public slots:
    void closeEvent(QCloseEvent *);

    void hideShow(QSystemTrayIcon::ActivationReason reason = QSystemTrayIcon::Unknown);

protected:
    void createIcon();
    void createActions();
    void createTrayIcon();

private:
    QIcon *icon;

    QAction *hideShowAction;
    QAction *quitAction;

    QMenu *trayIconMenu;

    QSystemTrayIcon *trayIcon;
};

#endif // BURGOS_H
