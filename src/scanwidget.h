#ifndef SCANWIDGET_H
#define SCANWIDGET_H

#include "ui_scanwidget.h"
#include "ftpmodel.h"
#include "scanall.h"

#include <QWidget>
#include <QTreeWidget>
#include <QNetworkAddressEntry>

class ScanWidget : public QWidget
{
    Q_OBJECT
public:
    ScanWidget(FtpModel *ftpModel = 0 , QWidget *parent = 0);

private slots:
    void on_pushButton_clicked();
    void reactiveButton();

private:
    ScanAll *newScanAll();

    Ui::ScanWidget ui;

    QMap<QTreeWidgetItem *, QNetworkAddressEntry> items;
    FtpModel *ftpModel;
};

#endif // SCANWIDGET_H
