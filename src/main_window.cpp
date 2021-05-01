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
#include "device_table_view.h"
#include "main_window.h"

#include <QDebug>
#include <QDockWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("searchaBLE");
    setContentsMargins(20, 4, 20, 20);
    // TODO: this might need to be tuned
    setMinimumSize(700, 450);

    m_device_table = new DeviceTableView(this);
    connect(m_device_table, &QTableWidget::itemSelectionChanged, this, &MainWindow::device_selection_changed);

    m_connect_button = new QPushButton("connect");
    // button will not be enabled until a device is selected
    m_connect_button->setEnabled(false);
    connect(m_connect_button, &QPushButton::clicked, [this](bool) {
        connect_to_device(m_device_table->selected_device());
    });

    // wrap the button in a widget so that it is centered
    auto button_wrapper = new QWidget();
    auto layout = new QVBoxLayout();
    layout->addWidget(m_connect_button);
    layout->setAlignment(Qt::AlignCenter);
    button_wrapper->setLayout(layout);
    button_wrapper->setMinimumWidth(size().width());

    // place the "connect to device" button in the dock area
    auto dock = new QDockWidget();
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    dock->setWidget(button_wrapper);
    addDockWidget(Qt::BottomDockWidgetArea, dock);

    setCentralWidget(m_device_table);
}

void MainWindow::device_selection_changed()
{
    // enable the button and add the device name to its text
    m_connect_button->setEnabled(true);

    auto name = m_device_table->selected_device().name();
    auto button_text = QString("connect to %1").arg(name.isEmpty() ? "device" : name);
    m_connect_button->setText(button_text);
}

void MainWindow::connect_to_device(const QBluetoothDeviceInfo& device)
{
    qDebug() << "connecting to device " << device.name();

    // clear any previously-discovered services
    m_services.clear();

    // initialize the BLE controller with the device
    m_controller = std::unique_ptr<QLowEnergyController>(QLowEnergyController::createCentral(device));
    initialize_controller();

    // create the service window
    m_service_window = std::make_unique<ServiceView>();

    // set the window title while we have the device in scope
    auto title = device.name().isEmpty() ? "UNNAMED DEVICE" : device.name();
    m_service_window->set_window_title(title);

    // show the window while connecting to the device
    m_service_window->show();

    m_controller->connectToDevice();
}


void MainWindow::initialize_controller()
{
    // add services to the service window once the scan is complete
    connect(m_controller.get(), &QLowEnergyController::discoveryFinished, [this]() {
        qDebug() << "service discovery complete";
        m_service_window->set_services(m_services);
    });

    // close the service window when disconnected from the device
    connect(m_controller.get(), &QLowEnergyController::disconnected, [this]() {
        qDebug() << "controller disconnected from device";
        m_service_window.reset();
        m_controller.reset();
    });

    // search for services once connected to the device
    connect(m_controller.get(), &QLowEnergyController::connected, [this]() {
        qDebug() << "controller connected to device, searching for services";
        m_controller->discoverServices();
    });

    // track discovered services
    connect(m_controller.get(), &QLowEnergyController::serviceDiscovered, [this](auto uuid) {
        qDebug() << "discovered service" << uuid_to_string(uuid).c_str();
        auto service = m_controller->createServiceObject(uuid);
        m_services.emplace_back(service);
    });

    // show a dialog on error
    connect(m_controller.get(), static_cast<void (QLowEnergyController::*)(QLowEnergyController::Error)>(&QLowEnergyController::error), [this](QLowEnergyController::Error error) {
        QMessageBox alert;
        alert.setIcon(QMessageBox::Warning);
        alert.setText("An error occurred while connecting to the device");
        alert.setInformativeText(controller_error_into_string(std::move(error)));
        alert.exec();
        // also delete the service window and controller
        m_service_window.reset();
        m_controller->disconnectFromDevice();
        m_controller.reset();
    });
}
