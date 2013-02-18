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
#include "toggltask.h"
#include "togglconnector.h"

namespace Toggl {

TogglTask::TogglTask(TogglConnector *c) : TogglObject(c)
{

}
QString TogglTask::name() const
{
    return m_name;
}
TogglWorkspace *TogglTask::workspace() const
{
    return m_workspace;
}
TogglProject *TogglTask::project() const
{
    return m_project;
}
TogglUser *TogglTask::user() const
{
    return m_user;
}
int TogglTask::estimatedSeconds() const
{
    return m_estimatedSeconds;
}
bool TogglTask::isActive() const
{
    return m_active;
}
void TogglTask::setName(QString name)
{
    if(m_name!=name) {
        emit nameChanged(m_name = name);
        touch();
    }

}
void TogglTask::setWorkspace(TogglWorkspace *workspace)
{
    if(m_workspace!=workspace) {
        emit workspaceChanged(m_workspace = workspace);
        touch();
    }

}
void TogglTask::setProject(TogglProject *project)
{
    if(m_project!=project) {
        emit projectChanged(m_project = project);
        touch();
    }

}
void TogglTask::setUser(TogglUser *user)
{
    if(m_user!=user) {
        emit userChanged(m_user = user);
        touch();
    }

}
void TogglTask::setEstimatedSeconds(int secs)
{
    if(m_estimatedSeconds!=secs) {
        emit estimatedTimeChanged(m_estimatedSeconds = secs);
        touch();
    }

}
void TogglTask::setEstimatedHours(int hours)
{
    if(estimatedHours()!=hours) setEstimatedSeconds(hours*3600);
}
void TogglTask::setActive(bool active)
{
    if(m_active!=active) {
        emit isActiveChanged(m_active = active);
        touch();
    }

}

void TogglTask::save()
{
    TogglConnector *c = qobject_cast<TogglConnector*>(parent());
    if(c) c->uploadTask(this);
}

} // namespace Toggl
