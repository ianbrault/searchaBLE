#ifndef DEVICEAGENT_H
#define DEVICEAGENT_H

#include "bt_utils.h"

#include <QObject>
#include <QBluetoothDeviceDiscoveryAgent>

class DeviceAgent : public QObject
{
    Q_OBJECT

    std::unique_ptr<QBluetoothDeviceDiscoveryAgent> m_agent;
    // maps device addresses to info in order to filter out duplicates
    device_map_t m_devices;

public:
    DeviceAgent(int timeout);
    void scan_for_devices();

private:
    void add_device(const QBluetoothDeviceInfo& device);

signals:
    void scan_complete(device_map_t);
    void scan_error(QString);
};

#endif // DEVICEAGENT_H
