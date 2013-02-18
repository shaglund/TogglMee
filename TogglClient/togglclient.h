/* This file is part of of the TogglClient library
 *
 * Copyright (C) 2011 Keno Fischer <keno.fischer@gmx.de>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License version 2.1, as published by the Free Software Foundation.
 *
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

#ifndef TOGGL_TOGGLCLIENT_H
#define TOGGL_TOGGLCLIENT_H

#include "togglobject.h"

namespace Toggl {

class TogglWorkspace;

class TOGGLCLIENTSHARED_EXPORT TogglClient : public TogglObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Toggl::TogglWorkspace *workspace READ workspace WRITE setWorkspace NOTIFY workspaceChanged)
    Q_PROPERTY(double hourly_rate READ hourlyRate WRITE setHourlyRate NOTIFY hourlyRateChanged)
    Q_PROPERTY(QString currency READ currency WRITE setCurrency NOTIFY currencyChanged)

public:
    TogglClient(TogglConnector *c);
    QString name() const;
    TogglWorkspace *workspace() const;
    double hourlyRate() const;
    QString currency() const;

public slots:
    void save();
    void setName(QString name);
    void setWorkspace(TogglWorkspace *workspace);
    void setHourlyRate(double hourlyRate);
    void setCurrency(QString currency);

signals:
    void nameChanged(QString name);
    void workspaceChanged(TogglWorkspace *workspace);
    void hourlyRateChanged(double hourlyRate);
    void currencyChanged(QString currency);

private:
    QString m_name;
    TogglWorkspace *m_workspace;
    double m_hourlyRate;
    QString m_currency;
};

} // namespace Toggl

#endif // TOGGL_TOGGLCLIENT_H
