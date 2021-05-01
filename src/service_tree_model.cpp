/*
**    searchaBLE
**    Copyright (C) 2021  Ian Brault
**
**    This program is free software: you can redistribute it and/or modify
**    it under the terms of the GNU General Public License as published by
**    the Free Software Foundation, either version 3 of the License, or
**    (at your option) any later version.
**
**    This program is distributed in the hope that it will be useful,
**    but WITHOUT ANY WARRANTY; without even the implied warranty of
**    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**    GNU General Public License for more details.
**
**    You should have received a copy of the GNU General Public License
**    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "bt_utils.h"
#include "service_tree_item.h"
#include "service_tree_model.h"

#include <QDebug>

ServiceTreeModel::ServiceTreeModel(QObject* parent)
    : QAbstractItemModel(parent)
{
    root = new ServiceTreeItem("Service/Characteristic UUID");
}

ServiceTreeModel::~ServiceTreeModel()
{
    delete root;
}

void ServiceTreeModel::set_services(const std::vector<std::unique_ptr<QLowEnergyService>>& services)
{
    // clear out existing services
    root->clear_children();

    for (const auto& service : services)
    {
        // add an item to the root for each service
        qDebug() << "adding service" << uuid_to_string(service->serviceUuid()).c_str() << "to model";
        auto service_item = new ServiceTreeItem(service->serviceUuid(), root);
        root->append_child(service_item);

        // add characteristics as children of the service
        foreach (auto characteristic, service->characteristics())
        {
            qDebug() << "for service" << uuid_to_string(service->serviceUuid()).c_str()
                     << "adding characteristic" << uuid_to_string(characteristic.uuid()).c_str() << "to model";
            auto char_item = new ServiceTreeItem(characteristic.uuid(), service_item);
            service_item->append_child(char_item);
        }

        // if there are no characteristics, add a blank item
        if (service_item->child_count() == 0)
        {
            auto filler = new ServiceTreeItem({}, service_item);
            service_item->append_child(filler);
        }
    }
}

QModelIndex ServiceTreeModel::index(int row, int column, const QModelIndex& parent) const
{
    if (!hasIndex(row, column, parent))
    {
        return QModelIndex();
    }

    auto parent_item = parent.isValid() ? static_cast<ServiceTreeItem*>(parent.internalPointer()) : root;
    auto child_item = parent_item->child(row);

    return child_item ? createIndex(row, column, child_item) : QModelIndex();
}

QModelIndex ServiceTreeModel::parent(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return QModelIndex();
    }

    auto child_item = static_cast<ServiceTreeItem*>(index.internalPointer());
    auto parent_item = child_item->parent();

    return parent_item == root ? QModelIndex() : createIndex(parent_item->row(), 0, parent_item);
}

int ServiceTreeModel::rowCount(const QModelIndex& parent) const
{
    if (parent.column() > 0)
    {
        return 0;
    }

    auto parent_item = parent.isValid() ? static_cast<ServiceTreeItem*>(parent.internalPointer()) : root;
    return parent_item->child_count();
}

int ServiceTreeModel::columnCount(const QModelIndex&) const
{
    // NOTE: just the UUID for now
    return 1;
}

QVariant ServiceTreeModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
    {
        return QVariant();
    }

    auto item = static_cast<ServiceTreeItem*>(index.internalPointer());
    return item->data(index.column());
}

Qt::ItemFlags ServiceTreeModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
    {
        return Qt::NoItemFlags;
    }
    return QAbstractItemModel::flags(index);
}

QVariant ServiceTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return root->data(section);
    }
    return QVariant();
}
