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

#ifndef BTUTILS_H
#define BTUTILS_H

#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>

// maps device address to info in order to filter out duplicates
typedef std::map<std::string, QBluetoothDeviceInfo> device_map_t;

std::string address_to_string(const QBluetoothDeviceInfo& device);

QString device_discovery_error_into_string(QBluetoothDeviceDiscoveryAgent::Error&& error);

#endif // BTUTILS_H
