#include "pcappacktablemodel.h"
#include <time.h>

PcapPackTableModel::PcapPackTableModel(pcap_loader* loader, QObject *parent) :
    QAbstractTableModel(parent)
{
    this->headList << "No" << "Length" << "Time";
    this->loader = loader;
}

int PcapPackTableModel::rowCount(const QModelIndex &parent) const
{
    return loader != NULL ? loader->get_packs_count() : 0;
}

int PcapPackTableModel::columnCount(const QModelIndex &parent) const
{
    return this->headList.size();
}

QVariant PcapPackTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || this->loader==NULL)
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 0:
            return QString("%1").arg(index.row()+1);
        case 1:
            return QString("%1").arg(this->loader->get_pack(index.row()).length());
        case 2:
            int32_t time = this->loader->get_pack(index.row()).data.timestamp_ms;
            return QString() + ctime((const time_t*)&time);
        }
    }
    if(role == Qt::TextAlignmentRole && index.column()!= 0)//设置文字对齐
    {
        return (Qt::AlignHCenter+Qt::AlignVCenter);
    }

    return QVariant();
}

QVariant PcapPackTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role == Qt::DisplayRole
            && orientation == Qt::Horizontal //水平标题
            && section < this->headList.size()
            )
        return this->headList[section];

    return QVariant();
}
