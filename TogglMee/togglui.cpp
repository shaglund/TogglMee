#include "togglui.h"
#include <QMap>
#include <QDebug>
#include <QtDeclarative>

TogglUI::TogglUI(QByteArray api_token, QObject *parent) :
    QObject(parent),
    m_connector(new Toggl::TogglConnector(api_token)),
    m_user(m_connector),
    m_timeentries(QList<Toggl::TogglTimeEntry*>()),
    m_clients(QList<Toggl::TogglClient*>()),
    m_projects(QList<Toggl::TogglProject*>()),
    m_tasks(QList<Toggl::TogglTask*>()),
    m_workspaces(QList<Toggl::TogglWorkspace*>())
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

QList<Toggl::TogglTimeEntry*> TogglUI::time_entry_list() const
{
    return m_timeentries;
}
QList<Toggl::TogglClient*> TogglUI::client_list() const
{
    return m_clients;
}
QList<Toggl::TogglProject*> TogglUI::project_list() const
{
    return m_projects;
}
QList<Toggl::TogglTask*> TogglUI::task_list() const
{
    return m_tasks;
}
QList<Toggl::TogglWorkspace*> TogglUI::workspace_list() const
{
    return m_workspaces;
}

void TogglUI::updateTimeEntries()
{
    m_timeentries = m_connector->timeEntries().values();
    /*
    QMap<int, Toggl::TogglTimeEntry*> entries = m_connector->timeEntries();
    QMap<int, Toggl::TogglTimeEntry*>::iterator i;
    for(i=entries.begin(); i != entries.end(); i++) {
        Toggl::TogglTimeEntry *entry = i.value();
        m_timeentries.prepend(entry);
    }
    qDebug() << "TogglUI initialized";
    */
    emit timeEntriesLoaded();
}

void TogglUI::updateClients()
{
    m_clients = m_connector->clients().values();
}

void TogglUI::updateProjects()
{
    m_projects = m_connector->projects().values();
}

void TogglUI::updateTasks()
{
    m_tasks = m_connector->tasks().values();
}

void TogglUI::updateWorkspaces()
{
    m_workspaces = m_connector->worspaces().values();
}
