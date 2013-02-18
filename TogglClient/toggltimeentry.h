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
#ifndef TOGGL_TOGGLTIMEENTRY_H
#define TOGGL_TOGGLTIMEENTRY_H

#include <QDateTime>
#include <QStringList>

#include "togglobject.h"

namespace Toggl {

class TogglWorkspace;
class TogglProject;
class TogglTask;

class TOGGLCLIENTSHARED_EXPORT TogglTimeEntry : public TogglObject
{
    Q_OBJECT
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged)
    Q_PROPERTY(int duration READ duration WRITE setDuration NOTIFY durationChanged)
    Q_PROPERTY(QDateTime start READ startDateTime WRITE setStart NOTIFY startChanged)
    Q_PROPERTY(QDateTime stop READ stopDateTime WRITE setStop NOTIFY stopChanged)
    Q_PROPERTY(bool ignore_start_stop READ ignoreStartStop WRITE setIgnoreStartStop NOTIFY ignoreStartStopChanged)
    Q_PROPERTY(QStringList tag_names READ tags WRITE setTags NOTIFY tagsChanged)
    Q_PROPERTY(Toggl::TogglWorkspace *workspace READ workspace WRITE setWorkspace NOTIFY workspaceChanged)
    Q_PROPERTY(Toggl::TogglProject *project READ project WRITE setProject NOTIFY projectChanged)
    Q_PROPERTY(Toggl::TogglTask *task READ task WRITE setTask NOTIFY taskChanged)
    Q_PROPERTY(bool billable READ isBillable WRITE setBillable NOTIFY isBillableChanged)
    Q_PROPERTY(QString created_with READ createdWith WRITE setCreatedWith NOTIFY createdWithChanged)

public:
    TogglTimeEntry(TogglConnector *c);
    QString description() const;
    int duration() const;
    QDateTime startDateTime() const;
    QDateTime stopDateTime() const;
    bool ignoreStartStop() const;
    QStringList tags() const;
    TogglWorkspace *workspace() const;
    TogglProject   *project() const;
    TogglTask      *task() const;
    bool isBillable() const;
    QString createdWith() const;    

public slots:
    void save();
    void setDescription(QString description);
    void setDuration(int duration);
    void setStart(QDateTime start);
    void setStop(QDateTime stop);
    void setIgnoreStartStop(bool ignoreStartStop);
    void setTags(QStringList tags);
    void setWorkspace(TogglWorkspace *workspace);
    void setProject(TogglProject *project);
    void setTask(TogglTask *task);
    void setBillable(bool isBillable);
    void setCreatedWith(const QString& created);

signals:
    void descriptionChanged(QString description);
    void durationChanged(int duration);
    void startChanged(QDateTime start);
    void stopChanged(QDateTime stop);
    void ignoreStartStopChanged(bool ignoreStartStop);
    void tagsChanged(QStringList tags);
    void workspaceChanged(TogglWorkspace *workspace);
    void projectChanged(TogglProject *project);
    void taskChanged(TogglTask *task);
    void isBillableChanged(bool isBillable);
    void createdWithChanged(QString createdWith);

private:
    QString m_description;
    int m_duration;
    QDateTime m_start;
    QDateTime m_stop;
    bool m_ignore;
    QStringList m_tagList;
    TogglWorkspace *m_workspace;
    TogglProject *m_project;
    TogglTask *m_task;
    bool m_billable;
    QString m_createdwith;
};
} // namespace Toggl

#endif // TOGGL_TOGGLTIMEENTRY_H
