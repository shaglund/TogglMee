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

#ifndef TOGGLCONNECTOR_H
#define TOGGLCONNECTOR_H

#include <QObject>
#include <QMap>
#include <QVariant>
#include <QStringList>

#include <QNetworkAccessManager>

#include "TogglClient_global.h"


namespace Toggl
{
class TogglProject;
class TogglClient;
class TogglTask;
class TogglTimeEntry;
class TogglWorkspace;
class TogglUser;
class TogglObject;

enum Error { NoError, GeneralError, NetworkError, ParserError };

enum urlPos {
    SessionUrl          =   0,
    ProjectsUrl         =   1,
    TimeEntriesUrl      =   2,
    WorkspacesUrl       =   3,
    ClientsUrl          =   4,
    TasksUrl            =   5,
    TagsUrl             =   6,
    CurrentUserUrl      =   7,
    ProjectDetailUrl    =   8,
    ClientDetailUrl     =   9,
    TaskDetailUrl       = 0xA,
    TimeEntryDetailUrl  = 0xB
};

template <typename T> class ErrorHandler
{
public:
    ErrorHandler() : m_error((T)0)
    {}
    inline T error() const { return m_error; }
    QString errorString() const { return m_errorString; }
protected:
    void reportError(T errorType,QString error) {
        m_error=errorType;
        m_errorString=error;
        qWarning(error.toAscii().data());
    }
private:
    T m_error;
    QString m_errorString;
};

class TOGGLCLIENTSHARED_EXPORT TogglConnector : public QObject, ErrorHandler<Error>
{
    Q_OBJECT
    enum ObjectType { Project, Client, Task, TimeEntry, Workspace, User };
public:
    TogglConnector(QByteArray m_apiToken, QObject *parent = 0);
    ~TogglConnector();

    QMap<int,TogglProject*> projects();
    QMap<int,TogglClient*> clients();
    QMap<int,TogglWorkspace*> worspaces();
    QMap<int,TogglTask*> tasks();
    QMap<int,TogglTimeEntry*> timeEntries();
    QMap<int,TogglUser*> users();

    void uploadProject(TogglProject *project);
    void uploadClient(TogglClient *client);
    void uploadTask(TogglTask *task);
    void uploadTimeEntry(TogglTimeEntry *entry);
    /* Maybe later
    void uploadWorkspace(TogglWorkspace *worspaces);
    void uploadUser(TogglUser *user); */

    void deleteProject(TogglProject *project);
    void deleteClient(TogglClient *client);
    void deleteTask(TogglTask *task);
    void deleteTimeEntry(TogglTimeEntry *entry);

    QList<TogglProject*> findProjectByName(QString name);
    /* Maybe later
    void deleteWorkspace(TogglWorkspace *worspace);
    void deleteUser(TogglUser *user); */
public slots:
    void init();
    void loadProjects();

    void queryProject(int id);
    void queryClient(int id);
    void queryTask(int id);
    void queryTimeEntry(int id);
    void queryWorkspace(int id);
    void queryUser(int id);
    void queryObject(int id, ObjectType type);
private slots:
    void parseAuthResponse();
    void parseProject();
    void parseProjects();
    void parseProjects(const QVariant &data);
    void parseClient();
    void parseClients();
    void parseClients(const QVariant &data);
    void parseTask();
    void parseTasks();
    void parseTasks(const QVariant &data);
    void parseTimeEntry();
    void parseTimeEntries();
    void parseTimeEntries(const QVariant &data);
    void parseWorkspaces();
    void parseWorkspaces(const QVariant &data);
    void parseUsers();
    void parseUsers(const QVariant &data);
    void authenticate(QNetworkReply *reply, QAuthenticator *auth);

    void completeProjectDeletion();
    void completeClientDeletion();
    void completeTaskDeletion();
    void completeTimeEntryDeletion();

signals:
    void projectsLoaded();
    void clientsLoaded();
    void workspacesLoaded();
    void tasksLoaded();
    void timeEntriesLoaded();

private:
    template <typename T > void completeObjectDeletion(QMap<int,T*>* list);
    template <typename T> bool serializeSpecial(const QString &propertyName, QVariantMap &map, const QMetaObject *mo, TogglObject* object, const QStringList &ignores);
    template < typename T > T *parseGeneric(const QVariant &v, QMap<int,T*> &objectMap,void (TogglConnector::*queryFunction)(int),bool &isStub, bool &isUpdate, QVariantMap &map);
    template < typename T > inline T *parseGeneric(const QVariant &v, QMap<int,T*> &objectMap,void (TogglConnector::*queryFunction)(int)) {
        bool isStub;
        bool isUpdate;
        QVariantMap map;
        return parseGeneric<T>(v,objectMap,queryFunction,isStub,isUpdate,map);
    }

    QNetworkReply *query(QUrl);
    void update(const QUrl &url, const QVariantMap &map, const char *slot,TogglObject *object);

    QNetworkReply *validateReply();

    QVariantMap serializeObjectIdOnly(TogglObject *object);
    QVariantMap serializeObject(TogglObject *object,const QStringList &additionalIgnore);

    inline QVariantMap createMap(QString objectName,TogglObject *object,QStringList ignores = QStringList());

    void deleteObject(urlPos pos, TogglObject *object, const char *slot);

    TogglProject   *parseProject(const QVariant &v);
    TogglClient    *parseClient(const QVariant &v);
    TogglTask      *parseTask(const QVariant &v);
    TogglTimeEntry *parseTimeEntry(const QVariant &v);
    TogglWorkspace *parseWorkspace(const QVariant &v);
    TogglUser      *parseUser(const QVariant &v);
    void setPropertries(const QVariantMap &v, TogglObject *object);

    void addObjectToMap(TogglProject *project);
    void addObjectToMap(TogglClient *client);
    void addObjectToMap(TogglWorkspace *workspace);
    void addObjectToMap(TogglTask *task);
    void addObjectToMap(TogglTimeEntry *timeEntry);
    void addObjectToMap(TogglUser *user);
    void addGenericObjectToMap(TogglObject *object);

    QVariant parseResponse();
    QNetworkAccessManager _nam;
    QMap<ObjectType, QList<int>* > m_pendingQueries;
    QMap<int,TogglProject*> m_projects;
    QMap<int,TogglClient*> m_clients;
    QMap<int,TogglWorkspace*> m_workspaces;
    QMap<int,TogglTask*> m_tasks;
    QMap<int,TogglTimeEntry*> m_timeEntries;
    QMap<int,TogglUser*> m_users;

    QMap<QNetworkReply*, TogglObject*> m_uploadedObjects;

    QByteArray m_apiToken;
};

}

Q_DECLARE_METATYPE(Toggl::TogglClient*)
Q_DECLARE_METATYPE(Toggl::TogglWorkspace*)
Q_DECLARE_METATYPE(Toggl::TogglTask*)
Q_DECLARE_METATYPE(Toggl::TogglTimeEntry*)
Q_DECLARE_METATYPE(Toggl::TogglUser*)
Q_DECLARE_METATYPE(Toggl::TogglProject*)

#endif // TOGGLCONNECTOR_H
