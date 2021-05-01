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

#ifndef SERVICETREEITEM_H
#define SERVICETREEITEM_H

#include <QBluetoothUuid>
#include <QVariant>

class ServiceTreeItem
{
    ServiceTreeItem* m_parent;
    std::vector<ServiceTreeItem*> m_children;
    QBluetoothUuid m_uuid;
    QString m_name;
    // only used for the root item
    QString m_col_0;
    QString m_col_1;

public:
    ServiceTreeItem(const QBluetoothUuid& uuid, const QString& name, ServiceTreeItem *parent = nullptr);
    // this constructor should be used for the root
    ServiceTreeItem(QString col_0, QString col_1);
    ~ServiceTreeItem();

    void append_child(ServiceTreeItem* child);
    void clear_children();

    ServiceTreeItem* parent();
    ServiceTreeItem* child(int row);
    QVariant data(int column) const;

    QBluetoothUuid uuid() const;
    int row() const;
    int child_count() const;
    int column_count() const;
};

#endif // SERVICETREEITEM_H
