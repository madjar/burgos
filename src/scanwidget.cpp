#include "scanwidget.h"

#include <QHBoxLayout>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QStringList>
#include <QPair>
#include <QHeaderView>
#include <cmath>

ScanWidget::ScanWidget(FtpModel *ftpModel, QWidget *parent) :
        QWidget(parent), ftpModel(ftpModel), scanInProgress(false)
#if QT_VERSION >= 0x040500
        , addCustom(tr("Add custom scan"))
#endif
{
    ui.setupUi(this);

    ui.treeWidget->header()->setResizeMode(0,QHeaderView::ResizeToContents);
    ui.treeWidget->header()->setResizeMode(1,QHeaderView::ResizeToContents);
    ui.treeWidget->header()->setResizeMode(2,QHeaderView::ResizeToContents);

    // Select the interface with the smaller submask
    quint32 minSize = 0xFFFFFFFF;
    QTreeWidgetItem *maxItem = 0;

    foreach (QNetworkInterface iface, QNetworkInterface::allInterfaces())
    {
        foreach (QNetworkAddressEntry entry, iface.addressEntries())
        {
            if (ScanAll::isValid(entry))
            {
                QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeWidget);
#if QT_VERSION >= 0x040500
                item->setText(0, iface.humanReadableName());
#else
                item->setText(0, iface.name());
#endif
                item->setText(1, entry.ip().toString());
                quint32 size = ~(entry.netmask().toIPv4Address())+1;
                item->setText(2, QString::number (size));
                items[item] = entry;

                if (size < minSize)
                {
                    minSize = size;
                    maxItem = item;
                }
            }
        }
    }

    maxItem->setSelected(true);

#if QT_VERSION >= 0x040500 // Only for Qt 4.5 and later
    QTreeWidgetItem *item = new QTreeWidgetItem(ui.treeWidget, 1000);
    item->setFlags(Qt::ItemIsEditable | Qt::ItemIsEnabled);
    item->setFirstColumnSpanned(true);
    item->setText(0, addCustom);
    item->setToolTip(0, tr("You can use the following forms :"
"<ul><li> 123.123.123.123/n where n is any value between 0 and 32"
"<li> 123.123.123.123/255.255.255.255"
"<li> missing trailing components (i.e., less than 4 octets, like \"192.168.1\"), followed or not by a dot</ul>"));
#endif
}

void ScanWidget::on_pushButton_clicked()
{
    if (!scanInProgress)
    {
        QList<QNetworkAddressEntry> entries;
        foreach (QTreeWidgetItem *item, ui.treeWidget->selectedItems())
            entries.append(items.value(item));

        newScanAll()->scan(entries);
    }
}

void ScanWidget::reactiveButton()
{
    scanInProgress = false;
    ui.pushButton->setText(tr("Scan"));
    //TODO ce texte apparait �  trois endroits différents. (ui et ici)
}

ScanAll *ScanWidget::newScanAll()
{
    ui.pushButton->setText(tr("Cancel"));
    scanInProgress = true;
    ScanAll *scan = new ScanAll(this);
    connect(scan, SIGNAL(found(QString)),
            ftpModel, SLOT(addFtp(QString)));
    connect(scan, SIGNAL(maximumChanged(int)),
            ui.progressBar, SLOT(setMaximum(int)));
    connect(scan, SIGNAL(progressChanged(int)),
            ui.progressBar, SLOT(setValue(int)));
    connect(scan, SIGNAL(destroyed()),
            this, SLOT(reactiveButton()));
    connect(ui.pushButton, SIGNAL(clicked()),
            scan, SLOT(deleteLater()));
    return scan;
}

#if QT_VERSION >= 0x040500 // Only for Qt 4.5 and later
void ScanWidget::on_treeWidget_itemClicked(QTreeWidgetItem* item, int /*column*/)
{
    if (item->flags() & Qt::ItemIsEditable)
    {
        //That's the "Add custom scan" item
        ui.treeWidget->editItem(item);
    }
}

void ScanWidget::on_treeWidget_itemChanged(QTreeWidgetItem* item, int /*column*/)
{
    if (!(item->flags() & Qt::ItemIsEditable))
        return;
    if (item->text(0) == "")
        item->setText(0, addCustom);
    if (item->text(0) != addCustom)
    {
        QPair<QHostAddress, int> pair = QHostAddress::parseSubnet(item->text(0));
        if (pair.second == -1)
        {
            item->setText(0, tr("Invalid entry"));
        }
        else
        {
            QTreeWidgetItem *newItem = new QTreeWidgetItem();
            newItem->setText(0, tr("Custom"));
            newItem->setText(1, pair.first.toString());
            newItem->setText(2, QString::number(pow(2,32 - pair.second)));
            ui.treeWidget->insertTopLevelItem(
                    ui.treeWidget->indexOfTopLevelItem(item), newItem);

            QNetworkAddressEntry entry;
            entry.setIp(pair.first);
            entry.setPrefixLength(pair.second);
            items[newItem] = entry;
            item->setText(0, addCustom);
        }
    }
}
#endif