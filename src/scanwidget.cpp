#include "scanwidget.h"

#include <QHBoxLayout>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QStringList>

//TODO avertissements mask trop large
//TODO Scan avancé (221.182.182.192/21)

ScanWidget::ScanWidget(FtpModel *ftpModel, QWidget *parent) :
        QWidget(parent), ftpModel(ftpModel)
{
    ui.setupUi(this);

    ui.treeWidget->header()->setResizeMode(0,QHeaderView::ResizeToContents);
    ui.treeWidget->header()->setResizeMode(1,QHeaderView::ResizeToContents);
    ui.treeWidget->header()->setResizeMode(2,QHeaderView::ResizeToContents);

    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces())
    {
        foreach (QNetworkAddressEntry entry, iface.addressEntries())
        {
            if (ScanAll::isValid(entry))
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeWidget);
                item->setText(0, iface.humanReadableName());
                item->setText(1, entry.ip().toString());
                item->setText(2, QString::number (entry.netmask()
                                                  .toIPv4Address(),16));
                items[item] = entry;
            }
        }
    }
}

void ScanWidget::on_pushButton_clicked()
{
    ui.pushButton->setEnabled(false);
    QList<QNetworkAddressEntry> entries;
    foreach (QTreeWidgetItem *item, ui.treeWidget->selectedItems())
        entries.append(items.value(item));

    newScanAll()->scan(entries);
}

void ScanWidget::reactiveButton()
{
    ui.pushButton->setEnabled(true);
}

ScanAll *ScanWidget::newScanAll()
{
    ScanAll *scan = new ScanAll(this);
    connect(scan, SIGNAL(found(QString)),
            ftpModel, SLOT(addFtp(QString)));
    connect(scan, SIGNAL(maximumChanged(int)),
            ui.progressBar, SLOT(setMaximum(int)));
    connect(scan, SIGNAL(progressChanged(int)),
            ui.progressBar, SLOT(setValue(int)));
    connect(scan, SIGNAL(destroyed()),
            this, SLOT(reactiveButton()));
    return scan;
}
