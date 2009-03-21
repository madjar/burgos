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

    bool scanInProgress;

    QMap<QTreeWidgetItem *, QNetworkAddressEntry> items;
    FtpModel *ftpModel;

#if QT_VERSION >= 0x040500
private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
    void on_treeWidget_itemChanged(QTreeWidgetItem* item, int column);
private:
    const QString addCustom;
#endif
};

#endif // SCANWIDGET_H
