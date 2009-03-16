#ifndef SCANWIDGET_H
#define SCANWIDGET_H

#include "ui_scanwidget.h"

#include <QWidget>
#include <QTreeWidget>
#include <QNetworkAddressEntry>

class ScanWidget : public QWidget
{
    Q_OBJECT
public:
    ScanWidget(QWidget *parent = 0);

private slots:
    void on_pushButton_clicked();
    void reactiveButton();

private:
    Ui::ScanWidget ui;

    QMap<QTreeWidgetItem *, QNetworkAddressEntry> items;
};

#endif // SCANWIDGET_H
