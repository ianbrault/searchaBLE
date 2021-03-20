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

#include "device_agent.h"

DeviceAgent::DeviceAgent(int timeout)
    : QObject()
{
    m_agent = std::make_unique<QBluetoothDeviceDiscoveryAgent>();
    m_agent->setLowEnergyDiscoveryTimeout(timeout);

    // set up discovery agent slots
    connect(m_agent.get(), &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this, &DeviceAgent::add_device);
    connect(m_agent.get(), &QBluetoothDeviceDiscoveryAgent::finished, [this]() {
        emit scan_complete(m_devices);
    });
    connect(m_agent.get(), &QBluetoothDeviceDiscoveryAgent::canceled, [this]() {
        emit scan_complete(m_devices);
    });
    connect(m_agent.get(), static_cast<void (QBluetoothDeviceDiscoveryAgent::*)(QBluetoothDeviceDiscoveryAgent::Error)>(&QBluetoothDeviceDiscoveryAgent::error), [this](auto error) {
        emit scan_error(device_discovery_error_into_string(std::move(error)));
    });
}

void DeviceAgent::add_device(const QBluetoothDeviceInfo& device)
{
    if (device.isValid() && device.coreConfigurations() & QBluetoothDeviceInfo::LowEnergyCoreConfiguration)
    {
        auto address = address_to_string(device);
        m_devices.insert({address, device});
    }
}

void DeviceAgent::scan_for_devices()
{
    m_devices.clear();
    m_agent->start(QBluetoothDeviceDiscoveryAgent::LowEnergyMethod);
}
