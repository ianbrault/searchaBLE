#include <QBluetoothUuid>

#include <fmt/core.h>

#include "bt_utils.h"

std::string address_to_string(const QBluetoothDeviceInfo& device)
{
    std::string s;
#ifdef Q_OS_DARWIN
    auto uuid = device.deviceUuid();
    auto size = uuid.minimumSize();
    if (size == 2)
    {
        s = fmt::format("{:#04X}", uuid.toUInt16());
    }
    else if (size == 4)
    {
        s = fmt::format("{:#08X}", uuid.toUInt32());
    }
    else if (size == 16)
    {
        auto big = uuid.toUInt128();
        s = fmt::format(
            "{:02X}{:02X}{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}-{:02X}{:02X}{:02X}{:02X}{:02X}{:02X}",
            big.data[0], big.data[1], big.data[2], big.data[3], big.data[4], big.data[5], big.data[6], big.data[7],
            big.data[8], big.data[9], big.data[10], big.data[11], big.data[12], big.data[13], big.data[14], big.data[15]);
    }
#else
    s = device.address().toString().toStdString();
#endif
    return s;
}
