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

#ifndef SERVICEVIEW_H
#define SERVICEVIEW_H

#include <QLowEnergyService>
#include <QWidget>

class ServiceTreeView;

class ServiceView : public QWidget
{
    Q_OBJECT

    ServiceTreeView* m_service_tree;

public:
    ServiceView();

    void set_window_title(QString title);
    void set_services(const std::vector<std::unique_ptr<QLowEnergyService>>& services);
};

#endif // SERVICEVIEW_H
