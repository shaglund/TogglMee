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
    QList<Toggl::TogglTimeEntry*> time_entry_list();
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
    Toggl::TogglClient m_toggl;
    Toggl::TogglProject m_project;
    Toggl::TogglTask m_task;
    Toggl::TogglTimeEntry m_timeentry;
    Toggl::TogglUser m_user;
    Toggl::TogglWorkspace m_workspace;
    QList<Toggl::TogglTimeEntry*> m_timeentries;
    int m_loaded;
};

#endif // TOGGLUI_H
