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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "service_view.h"

#include <QBluetoothDeviceInfo>
#include <QLowEnergyController>
#include <QLowEnergyService>
#include <QMainWindow>

class DeviceTableView;
class QPushButton;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    DeviceTableView* m_device_table;
    QPushButton* m_connect_button;

    std::unique_ptr<QLowEnergyController> m_controller;
    std::unique_ptr<ServiceView> m_service_window;
    std::vector<std::unique_ptr<QLowEnergyService>> m_services;

public:
    MainWindow(QWidget *parent = nullptr);

private:
    void initialize_controller();
    void connect_to_device(const QBluetoothDeviceInfo& device);

protected slots:
    void device_selection_changed();
};

#endif // MAINWINDOW_H
