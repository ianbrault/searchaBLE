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

#include "service_tree_view.h"
#include "service_view.h"
#include "spinner.h"

#include <QHBoxLayout>

ServiceView::ServiceView()
    : QWidget() // this should always be a floating window, hence no parent
{
    m_service_tree = new ServiceTreeView(this);
    // note: do not show the service tree view until services have been added
    m_service_tree->hide();

    // show a spinner until services have been added
    m_spinner = new Spinner(this);
    m_spinner->setInnerRadius(6);
    m_spinner->setLineLength(12);
    m_spinner->setLineWidth(3);
    m_spinner->setNumberOfLines(12);
    m_spinner->setRoundness(70.0);
    m_spinner->start();

    auto layout = new QHBoxLayout();
    // add a bunch of padding so the window is reasonably large with just the spinner in it
    layout->setContentsMargins(100, 100, 100, 100);
    layout->addWidget(m_spinner);
    setLayout(layout);
}

void ServiceView::set_window_title(QString title)
{
    setWindowTitle(title);
}

void ServiceView::set_services(const std::vector<std::unique_ptr<QLowEnergyService>>& services)
{
    m_service_tree->set_services(services);

    // now we can show the service views (and remove the spinner)
    m_spinner->stop();
    layout()->setContentsMargins(12, 12, 12, 12);  // this is roughly the default
    layout()->removeWidget(m_spinner);
    layout()->addWidget(m_service_tree);
    m_service_tree->show();
}
