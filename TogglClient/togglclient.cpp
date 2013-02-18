/* This file is part of of the TogglClient library
 *
 * Copyright (C) 2011 Keno Fischer <keno.fischer@gmx.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#include "togglconnector.h"
#include "togglclient.h"

namespace Toggl {

TogglClient::TogglClient(TogglConnector *c) : TogglObject(c)
{
m_workspace=0;
m_hourlyRate=0.0;
}
QString TogglClient::name() const
{
    return m_name;
}
TogglWorkspace *TogglClient::workspace() const
{
    return m_workspace;
}
double TogglClient::hourlyRate() const
{
    return m_hourlyRate;
}
QString TogglClient::currency() const
{
    return m_currency;
}

void TogglClient::setName(QString name)
{
    if(m_name!=name) {
        emit nameChanged(m_name = name);
        touch();
    }
}
void TogglClient::setWorkspace(TogglWorkspace *workspace)
{
    if(m_workspace!=workspace) {
        emit workspaceChanged(m_workspace = workspace);
        touch();
    }
}
void TogglClient::setHourlyRate(double hourlyRate)
{
    if(m_hourlyRate!=hourlyRate) {
        emit hourlyRateChanged(m_hourlyRate = hourlyRate);
        touch();
    }
}
void TogglClient::setCurrency(QString currency)
{
    if(m_currency!=currency) {
        emit currencyChanged(m_currency = currency);
        touch();
    }
}

void TogglClient::save()
{
    TogglConnector *c = qobject_cast<TogglConnector*>(parent());
    if(c) c->uploadClient(this);
}

} // namespace Toggl
