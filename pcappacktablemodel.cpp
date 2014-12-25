#include "pcappacktablemodel.h"
#include <time.h>

PcapPackTableModel::PcapPackTableModel(QObject *parent) :
    QAbstractTableModel(parent)
{
    this->headList << "" << "No" << "Length" << "Time";
    this->loader = NULL;
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
    if(!index.isValid() || this->loader==NULL || index.row() >= this->loader->get_packs_count())
        return QVariant();

    if(role == Qt::DisplayRole)
    {
        switch(index.column())
        {
        case 1:
            return QString("%1").arg(index.row()+1);
        case 2:
            return QString("%1").arg(this->loader->get_pack(index.row()).length());
        case 3:
            int32_t time = this->loader->get_pack(index.row()).data.timestamp_ms;
            return QString() + ctime((const time_t*)&time);
        }
    }
    if(role == Qt::TextAlignmentRole && index.column()!= 0)//设置文字对齐
    {
        return (Qt::AlignHCenter+Qt::AlignVCenter);
    }
    if (role == Qt::CheckStateRole && index.column()== 0)
    {
        if(this->m_checkedlist.at(index.row()))
            return Qt::Checked;
        return Qt::Unchecked;
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

Qt::ItemFlags PcapPackTableModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return 0;

    if (index.column() == 0)
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsUserCheckable;

    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

bool PcapPackTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid())
        return false;
    if (role == Qt::CheckStateRole && index.column() == 0)
    {
        bool ischecked = false;
        if (value == Qt::Checked) //
        {
            ischecked = true;
        }
        this->m_checkedlist.at(index.row()) = ischecked;
    }
    return true;
}

vector<bool> PcapPackTableModel::get_checked_list()
{
    return this->m_checkedlist;
}

void PcapPackTableModel::set_loader(pcap_loader *loader)
{
    this->loader = loader;
    if(this->loader)
    {
        this->m_checkedlist.resize(this->loader->get_packs_count(),false);
    }
}

void PcapPackTableModel::selectAll()
{
    unsigned int i;
    for(i=0; i<this->m_checkedlist.size(); ++i)
    {
        this->m_checkedlist.at(i) = true;
    }
}

void PcapPackTableModel::unselectAll()
{
    unsigned int i;
    for(i=0; i<this->m_checkedlist.size(); ++i)
    {
        this->m_checkedlist.at(i) = false;
    }
}

void PcapPackTableModel::reverseSelect()
{
    unsigned int i;
    for(i=0; i<this->m_checkedlist.size(); ++i)
    {
        this->m_checkedlist.at(i) = !this->m_checkedlist.at(i);
    }
}
