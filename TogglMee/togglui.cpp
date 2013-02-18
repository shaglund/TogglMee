#include "togglui.h"
#include <QMap>
#include <QDebug>
#include <QtDeclarative>

TogglUI::TogglUI(QByteArray api_token, QObject *parent) :
    QObject(parent),
    m_connector(new Toggl::TogglConnector(api_token)),
    m_toggl(m_connector),
    m_project(m_connector),
    m_task(m_connector),
    m_timeentry(m_connector),
    m_user(m_connector),
    m_workspace(m_connector),
    m_timeentries(QList<Toggl::TogglTimeEntry*>()),
    m_loaded(0)
{
    qDebug() << "TogglUI constructed";
    connect(m_connector, SIGNAL(timeEntriesLoaded()), this, SLOT(updateTimeEntries()));
    connect(m_connector, SIGNAL(clientsLoaded()), this, SLOT(updateClients()));
    connect(m_connector, SIGNAL(projectsLoaded()), this, SLOT(updateProjects()));
    connect(m_connector, SIGNAL(tasksLoaded()), this, SLOT(updateTasks()));
    connect(m_connector, SIGNAL(workspacesLoaded()), this, SLOT(updateWorkspaces()));
}

void TogglUI::init()
{
    qDebug() << "TogglUI initializing..";
    m_connector->init();
}

QList<Toggl::TogglTimeEntry*> TogglUI::time_entry_list()
{
    return m_timeentries;
}

void TogglUI::updateTimeEntries()
{
    QMap<int, Toggl::TogglTimeEntry*> entries = m_connector->timeEntries();
    QMap<int, Toggl::TogglTimeEntry*>::iterator i;
    for(i=entries.begin(); i != entries.end(); i++) {
        Toggl::TogglTimeEntry *entry = i.value();
        // qDebug() << i.key() << ": " << entry->description() << entry->duration() << entry->project()->name();
        m_timeentries.prepend(entry);
    }
    qDebug() << "TogglUI initialized";
    emit timeEntriesLoaded();
}

void TogglUI::updateClients()
{
    QMap<int, Toggl::TogglClient*> clients = m_connector->clients();
    QMap<int, Toggl::TogglClient*>::iterator i;
    for(i=clients.begin(); i!=clients.end(); i++) {
        Toggl::TogglClient *client = i.value();
        qDebug() << i.key() << ": " << client->name();
    }
}

void TogglUI::updateProjects()
{
    QMap<int, Toggl::TogglProject*> projects = m_connector->projects();
    QMap<int, Toggl::TogglProject*>::iterator i;
    for(i=projects.begin(); i!=projects.end(); i++) {
        Toggl::TogglProject *project = i.value();
        qDebug() << i.key() << ": " << project->name();
    }
}

void TogglUI::updateTasks()
{
    QMap<int, Toggl::TogglTask*> tasks = m_connector->tasks();
    QMap<int, Toggl::TogglTask*>::iterator i;
    for(i=tasks.begin(); i!=tasks.end(); i++) {
        Toggl::TogglTask *task = i.value();
        qDebug() << i.key() << ": " << task->name();
    }
}

void TogglUI::updateWorkspaces()
{
    QMap<int, Toggl::TogglWorkspace*> workspaces = m_connector->worspaces();
    QMap<int, Toggl::TogglWorkspace*>::iterator i;
    for(i=workspaces.begin(); i!=workspaces.end(); i++) {
        Toggl::TogglWorkspace *workspace = i.value();
        qDebug() << i.key() << ": " << workspace->name();
    }
}
