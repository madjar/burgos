#include "scanwidget.h"
#include "scanall.h"

#include <QHBoxLayout>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QStringList>

ScanWidget::ScanWidget(FtpModel *ftpModel, QWidget *parent) :
        QWidget(parent), ftpModel(ftpModel)
{
    ui.setupUi(this);

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

    ScanAll *scan = new ScanAll(this);
    connect(scan, SIGNAL(found(QString)),
            ftpModel, SLOT(addFtp(QString)));
    connect(scan, SIGNAL(maximumChanged(int)),
            ui.progressBar, SLOT(setMaximum(int)));
    connect(scan, SIGNAL(progressChanged(int)),
            ui.progressBar, SLOT(setValue(int)));
    connect(scan, SIGNAL(destroyed()),
            this, SLOT(reactiveButton()));

    scan->scan(entries);
}

void ScanWidget::reactiveButton()
{
    ui.pushButton->setEnabled(true);
}
