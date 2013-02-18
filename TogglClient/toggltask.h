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
#ifndef TOGGL_TOGGLTASK_H
#define TOGGL_TOGGLTASK_H

#include "togglobject.h"

namespace Toggl {

class TogglWorkspace;
class TogglProject;
class TogglUser;

class TOGGLCLIENTSHARED_EXPORT TogglTask : public TogglObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)
    Q_PROPERTY(Toggl::TogglWorkspace *workspace READ workspace WRITE setWorkspace NOTIFY workspaceChanged)
    Q_PROPERTY(Toggl::TogglProject *project READ project WRITE setProject NOTIFY projectChanged)
    Q_PROPERTY(Toggl::TogglUser *user READ user WRITE setUser NOTIFY userChanged)
    Q_PROPERTY(int estimated_seconds READ estimatedSeconds WRITE setEstimatedSeconds NOTIFY estimatedTimeChanged)
    Q_PROPERTY(int estimated_hours   READ estimatedHours   WRITE setEstimatedHours   NOTIFY estimatedTimeChanged)
    Q_PROPERTY(bool is_active READ isActive WRITE setActive NOTIFY isActiveChanged)

public:
    TogglTask(TogglConnector *c);
    QString name() const;
    TogglWorkspace *workspace() const;
    TogglProject *project() const;
    TogglUser *user() const;
    int estimatedSeconds() const;
    inline int estimatedHours() const { return (int) estimatedSeconds()/3600; }
    bool isActive () const;

public slots:
    void save();
    void setName(QString name);
    void setWorkspace(TogglWorkspace *workspace);
    void setProject(TogglProject *project);
    void setUser(TogglUser *user);
    void setEstimatedSeconds(int secs);
    void setEstimatedHours(int hours);
    void setActive(bool active);

signals:
    void nameChanged(QString name);
    void workspaceChanged(TogglWorkspace *workspace);
    void projectChanged(TogglProject *project);
    void userChanged(TogglUser *user);
    void estimatedTimeChanged(int seconds);
    void isActiveChanged(bool isActive);

private:
    QString m_name;
    TogglWorkspace *m_workspace;
    TogglProject *m_project;
    TogglUser *m_user;
    int m_estimatedSeconds;
    bool m_active;
};
} // namespace Toggl

#endif // TOGGL_TOGGLTASK_H
