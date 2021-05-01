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

#include <algorithm>

ServiceTreeItem::ServiceTreeItem(const QBluetoothUuid& uuid, ServiceTreeItem* parent)
    : m_parent(parent), m_uuid(uuid)
{

}

ServiceTreeItem::ServiceTreeItem(QString label)
    : m_parent(nullptr), m_label(label)
{

}

ServiceTreeItem::~ServiceTreeItem()
{
    for (auto* child : m_children)
    {
        delete child;
    }
}

void ServiceTreeItem::append_child(ServiceTreeItem* item)
{
    m_children.push_back(item);
}

void ServiceTreeItem::clear_children()
{
    for (auto* child : m_children)
    {
        delete child;
    }
    m_children.clear();
}

ServiceTreeItem* ServiceTreeItem::parent()
{
    return m_parent;
}

ServiceTreeItem* ServiceTreeItem::child(int row)
{
    if (row < 0 || row >= (int) m_children.size())
    {
        return nullptr;
    }
    // check if the child is a filler item
    auto child = m_children.at(row);
    if (child->uuid().minimumSize() == 0)
    {
        return nullptr;
    }
    else
    {
        return child;
    }
}

QVariant ServiceTreeItem::data(int column) const
{
    if (column == 0)
    {
        if (!m_label.isEmpty())
        {
            return m_label;
        }
        else
        {
            return QString::fromStdString(uuid_to_string(m_uuid));
        }
    }
    else
    {
        return QVariant();
    }
}

QBluetoothUuid ServiceTreeItem::uuid() const
{
    return m_uuid;
}

int ServiceTreeItem::row() const
{
    if (m_parent)
    {
        auto it = std::find(m_parent->m_children.begin(), m_parent->m_children.end(), this);
        if (it != m_parent->m_children.end())
        {
            return std::distance(m_parent->m_children.begin(), it);
        }
    }
    return 0;
}

int ServiceTreeItem::child_count() const
{
    return m_children.size();
}

int ServiceTreeItem::column_count() const
{
    // NOTE: just the UUID for now
    return 1;
}
