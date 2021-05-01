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

#ifndef SERVICETREEVIEW_H
#define SERVICETREEVIEW_H

#include <QLowEnergyService>
#include <QTreeView>

class ServiceTreeModel;

class ServiceTreeView : public QTreeView
{
    ServiceTreeModel* m_model;

public:
    ServiceTreeView(QWidget* parent = nullptr);

    void set_services(const std::vector<std::unique_ptr<QLowEnergyService>>& services);
};

#endif // SERVICETREEVIEW_H
