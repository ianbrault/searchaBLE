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

#include <fmt/core.h>

std::string uuid_to_string(const QBluetoothUuid& uuid)
{
    std::string s;

    auto size = uuid.minimumSize();
    if (size == 2)
    {
        s = fmt::format("0x{:04X}", uuid.toUInt16());
    }
    else if (size == 4)
    {
        s = fmt::format("0x{:08X}", uuid.toUInt32());
    }
    else if (size == 16)
    {
        auto big = uuid.toUInt128();
        s = fmt::format(
            "{:02X}{:02X}{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
            big.data[0], big.data[1], big.data[2], big.data[3], big.data[4], big.data[5], big.data[6], big.data[7],
            big.data[8], big.data[9], big.data[10], big.data[11], big.data[12], big.data[13], big.data[14], big.data[15]);
    }

    return s;
}

std::string address_to_string(const QBluetoothDeviceInfo& device)
{
    std::string s;

#ifdef Q_OS_DARWIN
    s = uuid_to_string(device.deviceUuid());
#else
    s = device.address().toString().toStdString();
#endif

    return s;
}

QString device_discovery_error_into_string(QBluetoothDeviceDiscoveryAgent::Error&& error)
{
    QString s;

    switch (error) {
    case QBluetoothDeviceDiscoveryAgent::Error::NoError:
        break;
    case QBluetoothDeviceDiscoveryAgent::Error::PoweredOffError:
        s = "The Bluetooth adaptor is powered off";
        break;
    case QBluetoothDeviceDiscoveryAgent::Error::InputOutputError:
        s = "Error reading from or writing to the Bluetooth adapter";
        break;
    case QBluetoothDeviceDiscoveryAgent::Error::InvalidBluetoothAdapterError:
        s = "The Bluetooth adapter is misconfigured";
        break;
    case QBluetoothDeviceDiscoveryAgent::Error::UnsupportedPlatformError:
        s = "Bluetooth is not supported on this platform";
        break;
    case QBluetoothDeviceDiscoveryAgent::Error::UnsupportedDiscoveryMethod:
        s = "Bluetooth Low Energy is not supported on this platform";
        break;
    case QBluetoothDeviceDiscoveryAgent::Error::UnknownError:
        s = "An unknown error occurred";
        break;
    };

    return s;
}

QString controller_error_into_string(QLowEnergyController::Error&& error)
{
    QString s;

    switch (error) {
    case QLowEnergyController::Error::NoError:
        break;
    case QLowEnergyController::Error::UnknownError:
        s = "An unknown error occurred";
        break;
    case QLowEnergyController::Error::UnknownRemoteDeviceError:
        s = "The device could not be found";
        break;
    case QLowEnergyController::Error::NetworkError:
        s = "Failed to read from or write to the device";
        break;
    case QLowEnergyController::Error::InvalidBluetoothAdapterError:
        s = "The local Bluetooth adapter is invalid";
        break;
    case QLowEnergyController::Error::ConnectionError:
        s = "Failed to connect to the Bluetooth device";
        break;
    case QLowEnergyController::Error::AdvertisingError:
        s = "Failed to start advertising";
        break;
    case QLowEnergyController::Error::RemoteHostClosedError:
        s = "The remote device closed the connection";
        break;
    case QLowEnergyController::Error::AuthorizationError:
        s = "The local Bluetooth closed the connection due to insufficient authorization";
        break;
    };

    return s;
}
