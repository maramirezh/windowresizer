#include "windowresizer/processlistmodel.h"
#include "windowresizerutils/processutils.h"

using namespace winresizer;

ProcessListModel::ProcessListModel(std::unique_ptr<OSInterface> osInterface, QObject *parent)
    : QAbstractTableModel(parent),
    processUtils(std::move(osInterface)),
    processList(processUtils.getProcessDataList())
{
}

QVariant ProcessListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole || orientation == Qt::Orientation::Vertical) {
        return QVariant();
    }

    switch (section) {
    case ProcessListColumn::ProcessName:
        return "Process Name";
    case ProcessListColumn::ProcessId:
        return "Process ID";
    default:
        throw std::exception("field not implemented");
    }
}

int ProcessListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return static_cast<int>(this->processList.size());
}

int ProcessListModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return ProcessListColumn::Max;
}

QVariant ProcessListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole) {
        return QVariant();
    }

    switch (index.column()) {
    case ProcessListColumn::ProcessName:
    {
        auto processName = this->processList.at(index.row()).getProcessName();
        return QString::fromStdString(processName);
    }
    case ProcessListColumn::ProcessId:
    {
        auto processId = this->processList.at(index.row()).getProcessId();
        return QString::number(processId);
    }
    default:
        throw std::exception("field not implemented");
    }
}

void ProcessListModel::resetModel()
{
    this->beginResetModel();
    this->processList = processUtils.getProcessDataList();
    this->endResetModel();
}
