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
#ifndef TOGGL_TOGGLPROJECT_H
#define TOGGL_TOGGLPROJECT_H

#include "TogglClient_global.h"
#include "togglobject.h"

namespace Toggl {

class TogglWorkspace;
class TogglClient;

class TOGGLCLIENTSHARED_EXPORT TogglProject : public TogglObject
{
public:
    enum BillingMethod { FixedFee, HourlyRate };
    enum EstimationMethod { FixedHours, Auto };
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(bool billable READ isBillable WRITE setBillable NOTIFY isBillableChanged)
    Q_PROPERTY(Toggl::TogglWorkspace *workspace READ workspace WRITE setWorkspace NOTIFY workspaceChanged)
    Q_PROPERTY(Toggl::TogglClient *client READ client WRITE setClient NOTIFY clientChanged)
    Q_PROPERTY(double fixed_fee READ fixedFee WRITE setFixedFee NOTIFY fixedFeeChanged)
    Q_PROPERTY(double hourly_rate READ hourlyRate WRITE setHourlyRate NOTIFY hourlyRateChanged)
    Q_PROPERTY(BillingMethod billingMethod READ billingMethod WRITE setBillingMethod NOTIFY billingMethodChanged)
    Q_PROPERTY(QString client_project_name READ clientProjectName WRITE setClientProjectName NOTIFY clientProjectNameChanged)
    Q_PROPERTY(int estimated_workhours READ estimatedWorkhours WRITE setEstimatedWorkhours NOTIFY estimatedWorkhoursChanged)
    Q_PROPERTY(EstimationMethod estimationMethod READ estimationMethod WRITE setEstimationMethod NOTIFY estimationMethodChanged)

public:
    TogglProject(TogglConnector *c);

    QString name() const;
    bool isBillable() const;
    TogglWorkspace *workspace() const;
    double fixedFee() const;
    double hourlyRate() const;
    BillingMethod billingMethod() const;
    QString clientProjectName() const;
    int estimatedWorkhours() const;
    EstimationMethod estimationMethod() const;
    TogglClient *client() const;

public slots:
    void save();
    void setName(QString name);
    void setBillable(bool isBillable);
    void setWorkspace(TogglWorkspace *workspace);
    void setFixedFee(double fee);
    void setHourlyRate(double rate);
    void setBillingMethod(BillingMethod method);
    void setClientProjectName(QString name);
    void setEstimatedWorkhours(int estimatedWorkhours);
    void setEstimationMethod(EstimationMethod method);
    void setClient(TogglClient *client);

signals:
    void nameChanged(QString name);
    void isBillableChanged(bool isBillable);
    void workspaceChanged(TogglWorkspace *workspace);
    void fixedFeeChanged(double fee);
    void hourlyRateChanged(double rate);
    void billingMethodChanged(BillingMethod method);
    void clientProjectNameChanged(QString name);
    void estimatedWorkhoursChanged(int workhours);
    void estimationMethodChanged(EstimationMethod method);
    void clientChanged(TogglClient *client);

private:
   QString m_name;
   bool m_billable;
   TogglWorkspace *m_workspace;
   TogglClient *m_client;
   double m_fixedFee;
   double m_hourlyRate;
   BillingMethod m_billingMethod;
   QString m_clientProjectName;
   int m_estimatedWorkhours;
   EstimationMethod m_estimationMethod;
};

} // namespace Toggl


#endif // TOGGL_TOGGLPROJECT_H
