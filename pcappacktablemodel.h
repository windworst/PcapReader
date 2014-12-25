#ifndef PCAPPACKTABLEMODEL_H
#define PCAPPACKTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>

#include "pcap_loader.h"

class PcapPackTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit PcapPackTableModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;              //重载QAbstractItemModel的rowCount函数
    int columnCount(const QModelIndex &parent = QModelIndex()) const;           //重载QAbstractItemModel的columnCount函数
    QVariant data(const QModelIndex &index, int role) const;                    //重载QAbstractItemModel的data函数
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;      //重载QAbstractItemModel的headerData函数
    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    vector<bool> get_checked_list();
    void set_loader(pcap_loader* loader);
signals:

public slots:
    void selectAll();
    void unselectAll();
    void reverseSelect();

private:
    QStringList headList;
    pcap_loader* loader;
    vector<bool>m_checkedlist;
};

#endif // PCAPPACKTABLEMODEL_H
