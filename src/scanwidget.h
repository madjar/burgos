/*

Copyright 2008 by Thomas Duboucher and Georges Dubus.

thomas.duboucher@supelec.fr
georges.dubus@supelec.fr

This software is a computer program whose purpose is to scan and
index ftp servers.

This software is governed by the CeCILL license under French law and
abiding by the rules of distribution of free software.  You can  use, 
modify and/ or redistribute the software under the terms of the CeCILL
license as circulated by CEA, CNRS and INRIA at the following URL
"http://www.cecill.info". 

As a counterpart to the access to the source code and  rights to copy,
modify and redistribute granted by the license, users are provided only
with a limited warranty  and the software's author,  the holder of the
economic rights,  and the successive licensors  have only  limited
liability. 

In this respect, the user's attention is drawn to the risks associated
with loading,  using,  modifying and/or developing or reproducing the
software by the user in light of its specific status of free software,
that may mean  that it is complicated to manipulate,  and  that  also
therefore means  that it is reserved for developers  and  experienced
professionals having in-depth computer knowledge. Users are therefore
encouraged to load and test the software's suitability as regards their
requirements in conditions enabling the security of their systems and/or 
data to be ensured and,  more generally, to use and operate it in the 
same conditions as regards security. 

The fact that you are presently reading this means that you have had
knowledge of the CeCILL license and that you accept its terms.

*/

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
    bool scanInProgress;

#if QT_VERSION >= 0x040500
private slots:
    void on_treeWidget_itemClicked(QTreeWidgetItem* item, int column);
    void on_treeWidget_itemChanged(QTreeWidgetItem* item, int column);
private:
    const QString addCustom;
#endif
};

#endif // SCANWIDGET_H
