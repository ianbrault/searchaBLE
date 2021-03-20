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
#include "device_table_view.h"

#include <QHeaderView>
#include <QLabel>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QTimer>
#include <QVBoxLayout>

#define DPR 3.0

Placeholder::Placeholder(QWidget* parent)
    : QWidget(parent)
{
    auto pixmap = QPixmap(":/images/logo.main.png");
    pixmap.setDevicePixelRatio(DPR);

    auto logo = new QLabel();
    logo->setScaledContents(true);
    logo->setPixmap(pixmap.scaled(Width * DPR, Width * DPR, Qt::KeepAspectRatio, Qt::SmoothTransformation));

    auto button = new QPushButton("search for devices");
    button->setMaximumWidth(180);
    connect(button, &QPushButton::clicked, [this](bool) {
        emit search_button_pressed();
    });

    auto layout = new QVBoxLayout();
    layout->addWidget(logo);
    layout->addSpacing(32);
    layout->addWidget(button, 0, Qt::AlignCenter);
    setLayout(layout);
}

static QTableWidgetItem* get_table_item(QString text)
{
    auto item = new QTableWidgetItem(text);
    // items should not be editable
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));

    return item;
}

DeviceTableView::DeviceTableView(QWidget* parent)
    : QTableWidget(parent)
{
    if (parent)
    {
        setMinimumSize(parent->size());
    }
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setSelectionMode(QAbstractItemView::SingleSelection);

    // allow columns to be sorted
    connect(horizontalHeader(), &QHeaderView::sortIndicatorChanged, this, &QTableWidget::sortItems);

    // initialize the device agent
    m_agent = std::make_unique<DeviceAgent>(Timeout);
    connect(m_agent.get(), &DeviceAgent::scan_complete, this, &DeviceTableView::set_devices);
    connect(m_agent.get(), &DeviceAgent::scan_error, this, &DeviceTableView::search_error);

    // initialize the placeholder widget
    m_placeholder = new Placeholder(this);
    connect(m_placeholder, &Placeholder::search_button_pressed, this, &DeviceTableView::search_for_devices);

    // initialize the progress bar and timer
    m_progress = new QProgressBar(this);
    m_progress->setVisible(false);
    m_progress->setMaximum(Timeout);
    m_progress->setMinimumWidth(size().width() / 3);
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, [this]() {
        m_progress->setValue(m_progress->value() + TimerInteral);
    });

    // initialize the table with an empty set of data
    set_devices({});
}

void DeviceTableView::set_header()
{
    setColumnCount(NColumns);

    QStringList column_headers = {"Device Name", "Device UUID"};
    setHorizontalHeaderLabels(column_headers);

    horizontalHeader()->setDefaultSectionSize(320);
    horizontalHeader()->setStretchLastSection(true);
    horizontalHeader()->setSortIndicatorShown(true);
}

void DeviceTableView::center_item(QWidget* widget)
{
    auto x = (size().width() - widget->size().width()) / 2;
    auto y = (size().height() - widget->size().height()) / 2;
    widget->move(x, y);
}

void DeviceTableView::clear_table()
{
    clear();
    set_header();
    // show the placeholder widget and center it
    m_progress->setVisible(false);
    m_placeholder->setVisible(true);
    center_item(m_placeholder);
}

void DeviceTableView::set_devices(const device_map_t& devices)
{
    m_timer->stop();

    if (devices.empty())
    {
        clear_table();
    }
    else
    {
        // hide the progress bar
        m_progress->setVisible(false);
        setRowCount(devices.size());

        auto row = 0;
        for (const auto& device : devices)
        {
            auto name = get_table_item(device.second.name());
            auto uuid = get_table_item(QString::fromStdString(device.first));
            setItem(row, 0, name);
            setItem(row, 1, uuid);
            row++;
        }
    }
}

void DeviceTableView::resizeEvent(QResizeEvent *event)
{
    // set to the new size
    resize(event->size());
    // re-center the placeholder and progress bar
    center_item(m_placeholder);
    center_item(m_progress);
}

void DeviceTableView::search_for_devices()
{
    // hide the placeholder, show the progress bar, and start the timer
    m_placeholder->setVisible(false);
    m_progress->setVisible(true);
    center_item(m_progress);
    m_timer->start(TimerInteral);

    m_agent->scan_for_devices();
}

void DeviceTableView::search_error(QString error)
{
    // stop the timer
    m_timer->stop();

    QMessageBox alert;
    alert.setIcon(QMessageBox::Warning);
    alert.setText("An error occurred while searching");
    alert.setInformativeText(error);
    alert.exec();

    // clear the table
    clear_table();
}
