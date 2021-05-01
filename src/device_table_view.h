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

#ifndef DEVICETABLE_H
#define DEVICETABLE_H

#include "bt_utils.h"

#include <QTableWidget>

class DeviceAgent;
class QProgressBar;
class QTimer;

class Placeholder : public QWidget
{
    Q_OBJECT

    const int Width = 300;

public:
    Placeholder(QWidget* parent = nullptr);

signals:
    void search_button_pressed();
};

class DeviceTableView : public QTableWidget
{
    Q_OBJECT

    const int NColumns = 2;
    const int Timeout = 3000;  // TODO: this should be controlled by a setting
    const int TimerInteral = 20;

    std::unique_ptr<DeviceAgent> m_agent;
    std::shared_ptr<device_map_t> m_devices;

    Placeholder* m_placeholder;
    QProgressBar* m_progress;
    QTimer* m_timer;

public:
    DeviceTableView(QWidget* parent = nullptr);
    void set_devices(const device_map_t& devices);
    QBluetoothDeviceInfo selected_device() const;

protected:
    void resizeEvent(QResizeEvent *event);

private:
    void set_header();
    void center_item(QWidget* item);
    void clear_table();

protected slots:
    void search_for_devices();
    void search_error(QString);
};

#endif // DEVICETABLE_H
