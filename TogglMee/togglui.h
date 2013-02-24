#ifndef TOGGLUI_H
#define TOGGLUI_H

#include <QObject>
#include <QList>
#include <togglclient.h>
#include <togglconnector.h>
#include <toggltimeentry.h>
#include <togglproject.h>
#include <toggltask.h>
#include <togglworkspace.h>
#include <toggluser.h>

class TogglUI : public QObject
{
    Q_OBJECT
public:
    explicit TogglUI(QByteArray api_token, QObject *parent = 0);
    void init();
    QList<Toggl::TogglTimeEntry*> time_entry_list() const;
    QList<Toggl::TogglClient*> client_list() const;
    QList<Toggl::TogglProject*> project_list() const;
    QList<Toggl::TogglTask*> task_list() const;
    QList<Toggl::TogglWorkspace*> workspace_list() const;
    Toggl::TogglConnector *togglConnector() {return m_connector;}
    
signals:
    void timeEntriesLoaded();

private slots:
    void updateTimeEntries();
    void updateProjects();
    void updateClients();
    void updateTasks();
    void updateWorkspaces();

private:    
    Toggl::TogglConnector *m_connector;
    Toggl::TogglUser m_user;
    QList<Toggl::TogglTimeEntry*> m_timeentries;
    QList<Toggl::TogglClient*> m_clients;
    QList<Toggl::TogglProject*> m_projects;
    QList<Toggl::TogglTask*> m_tasks;
    QList<Toggl::TogglWorkspace*> m_workspaces;
};

#endif // TOGGLUI_H
