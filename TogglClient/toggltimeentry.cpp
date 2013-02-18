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
#include <QDateTime>
#include "toggltimeentry.h"
#include "togglconnector.h"

namespace Toggl {

TogglTimeEntry::TogglTimeEntry(TogglConnector *c ) : TogglObject(c), m_workspace(NULL), m_project(NULL), m_task(NULL), m_createdwith("TogglClient")
{}
QString TogglTimeEntry::description() const
{
    return m_description;
}
int TogglTimeEntry::duration() const
{
    return m_duration;
}
QDateTime TogglTimeEntry::startDateTime() const
{
    return m_start;
}
QDateTime TogglTimeEntry::stopDateTime() const
{
    return m_stop;
}
bool TogglTimeEntry::ignoreStartStop() const
{
    return m_ignore;
}
QStringList TogglTimeEntry::tags() const
{
    return m_tagList;
}
TogglWorkspace *TogglTimeEntry::workspace() const
{
    return m_workspace;
}
TogglProject *TogglTimeEntry::project() const
{
    return m_project;
}
TogglTask *TogglTimeEntry::task() const
{
    return m_task;
}
bool TogglTimeEntry::isBillable() const
{
    return m_billable;
}
QString TogglTimeEntry::createdWith() const
{
    return m_createdwith;
}

void TogglTimeEntry::save()
{
    TogglConnector *c = qobject_cast<TogglConnector*>(parent());    
    if(c) c->uploadTimeEntry(this);
}

void TogglTimeEntry::setDescription(QString description)
{
    if(m_description!=description) {
        emit descriptionChanged(m_description = description);
        touch();
    }
}
void TogglTimeEntry::setDuration(int duration)
{
    if(m_duration!=duration) {
        emit durationChanged(m_duration = duration);
        touch();
    }
}
void TogglTimeEntry::setStart(QDateTime start)
{
    if(m_start!=start) {
        emit startChanged(m_start = start);
        touch();
    }
}
void TogglTimeEntry::setStop(QDateTime stop)
{
    if(m_stop!=stop) {
        emit stopChanged(m_stop = stop);
        touch();
    }
}
void TogglTimeEntry::setIgnoreStartStop(bool ignoreStartStop)
{
    if(m_ignore!=ignoreStartStop) {
        emit ignoreStartStopChanged(m_ignore = ignoreStartStop);
        touch();
    }
}
void TogglTimeEntry::setTags(QStringList tags)
{
    if(m_tagList!=tags) {
        emit tagsChanged(m_tagList = tags);
        touch();
    }
}
void TogglTimeEntry::setWorkspace(TogglWorkspace *workspace)
{
    if(m_workspace!=workspace) {
        emit workspaceChanged(m_workspace = workspace);
        touch();
    }
}
void TogglTimeEntry::setProject(TogglProject *project)
{
    if(m_project!=project) {
        emit projectChanged(m_project = project);
        touch();
    }
}
void TogglTimeEntry::setTask(TogglTask *task)
{
    if(m_task!=task) {
        emit taskChanged(m_task = task);
        touch();
    }
}
void TogglTimeEntry::setBillable(bool isBillable)
{
    if(m_billable != isBillable) {
        emit isBillableChanged(m_billable = isBillable);
        touch();
    }
}
void TogglTimeEntry::setCreatedWith(const QString& created)
{
    if(m_createdwith != created) {
        emit createdWithChanged(m_createdwith = created);
        touch();
    }
}


} // namespace Toggl
