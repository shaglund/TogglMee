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
#include "togglproject.h"

namespace Toggl {

TogglProject::TogglProject(TogglConnector *c) : TogglObject(c)
{
    m_name=QString();
    m_billable=false;
    m_workspace=0;
    m_client=0;
    m_fixedFee=0;
    m_hourlyRate=0;
    m_billingMethod=FixedFee;
    m_clientProjectName=QString();
    m_estimatedWorkhours=0.0;
    m_estimationMethod=Auto;
}

QString TogglProject::name() const
{
    return m_name;
}
bool TogglProject::isBillable() const
{
    return m_billable;
}
TogglWorkspace *TogglProject::workspace() const
{
    return m_workspace;
}
double TogglProject::fixedFee() const
{
    return m_fixedFee;
}
double TogglProject::hourlyRate() const
{
    return m_hourlyRate;
}
TogglProject::BillingMethod TogglProject::billingMethod() const
{
    return m_billingMethod;
}
QString TogglProject::clientProjectName() const
{
    return m_clientProjectName;
}
int TogglProject::estimatedWorkhours() const
{
    return m_estimatedWorkhours;
}
TogglProject::EstimationMethod TogglProject::estimationMethod() const
{
    return m_estimationMethod;
}
TogglClient *TogglProject::client() const
{
    return m_client;
}
void TogglProject::save()
{
    TogglConnector *c = qobject_cast<TogglConnector*>(parent());
    if(c) c->uploadProject(this);
}

void TogglProject::setName(QString name)
{
    if(m_name!=name) {
        emit nameChanged(m_name = name);
        touch();
    }

}

void TogglProject::setBillable(bool isBillable)
{
    if(isBillable!=m_billable) {
        emit isBillableChanged(m_billable = isBillable);
        touch();
    }

}

void TogglProject::setWorkspace(TogglWorkspace *workspace)
{
    if(workspace!=m_workspace) {
        emit workspaceChanged(m_workspace = workspace);
        touch();
    }

}

void TogglProject::setFixedFee(double fee)
{
    if(m_fixedFee!=fee) {
        emit fixedFeeChanged(m_fixedFee = fee);
        touch();
    }

}

void TogglProject::setHourlyRate(double rate)
{
    if(m_hourlyRate!=rate) {
        emit hourlyRateChanged(m_hourlyRate = rate);
        touch();
    }

}

void TogglProject::setBillingMethod(BillingMethod method)
{
    if(m_billingMethod!=method) {
        emit billingMethodChanged(m_billingMethod = method);
        touch();
    }

}

void TogglProject::setClientProjectName(QString name)
{
    if(m_clientProjectName!=name) {
        emit clientProjectNameChanged(m_clientProjectName = name);
        touch();
    }

}

void TogglProject::setEstimatedWorkhours(int estimatedWorkhours)
{
    if(m_estimatedWorkhours!=estimatedWorkhours) {
        emit estimatedWorkhoursChanged(m_estimatedWorkhours = estimatedWorkhours);
        touch();
    }
}

void TogglProject::setEstimationMethod(EstimationMethod method)
{
    if(m_estimationMethod!=method) {
        emit estimationMethodChanged(m_estimationMethod = method);
        touch();
    }
}

void TogglProject::setClient(TogglClient *client)
{
    if(m_client!=client)
    {
        emit clientChanged(m_client = client);
        touch();
    }
}
} // namespace Toggl
