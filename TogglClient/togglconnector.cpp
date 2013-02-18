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
#include <QDebug>

#include <QMetaProperty>

#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QAuthenticator>

//QJson
#include <QJson/Parser>
#include <QJson/QObjectHelper>
#include <QJson/Serializer>

#include "togglconnector.h"
#include "togglobject.h"
#include "togglworkspace.h"
#include "togglclient.h"
#include "togglproject.h"
#include "toggltask.h"
#include "toggluser.h"
#include "toggltimeentry.h"


namespace Toggl
{

static const char * const urls[] =
{
    "https://www.toggl.com/api/v6/sessions", //Session
    "https://www.toggl.com/api/v6/projects.json",  //Projects
    "https://www.toggl.com/api/v6/time_entries.json", //Time Entries
    "https://www.toggl.com/api/v6/workspaces.json", //Workspaces
    "https://www.toggl.com/api/v6/clients.json", //Clients
    "https://www.toggl.com/api/v6/tasks.json", //Tasks
    "https://www.toggl.com/api/v6/tags.json", //Tags
    "https://www.toggl.com/api/v6/me.json", //Current User
    "https://www.toggl.com/api/v6/projects/%1.json", //Projects Detail
    "https://www.toggl.com/api/v6/clients/%1.json", //Clients Details
    "https://www.toggl.com/api/v6/tasks/%1.json", //Tasks Detail
    "https://www.toggl.com/api/v6/time_entries/%1.json" //Time Entry Details

};

TogglConnector::TogglConnector(QByteArray token, QObject *parent) : QObject(parent), m_apiToken(token)
{
    connect(&_nam,SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)),SLOT(authenticate(QNetworkReply*,QAuthenticator*)));
    for(int i = 0; i < 7; ++i)
    {
        m_pendingQueries.insert(static_cast<ObjectType>(i),new QList<int>());
        m_pendingQueries.value(static_cast<ObjectType>(i))->append(0);
    }
    static bool initiated = false;
    if(!initiated)
    {
        qRegisterMetaType<TogglProject*>("Toggl::TogglProject*");
        qRegisterMetaType<TogglClient*>("Toggl::TogglClient*");
        qRegisterMetaType<TogglWorkspace*>("Toggl::TogglWorkspace*");
        qRegisterMetaType<TogglTask*>("Toggl::TogglTask*");
        qRegisterMetaType<TogglTimeEntry*>("Toggl::TogglTimeEntry*");
        qRegisterMetaType<TogglUser*>("Toggl::TogglUser*");
        initiated=true;
    }
}

TogglConnector::~TogglConnector()
{
    foreach(QList<int>*l,m_pendingQueries)
        delete l;
}

QNetworkReply *TogglConnector::query(QUrl url)
{
    QNetworkRequest request;
    request.setUrl(url);
    QNetworkReply *reply = _nam.get(request);
    reply->ignoreSslErrors();
    return reply;
}

void TogglConnector::init()
{
    QUrl url = QUrl(urls[SessionUrl]);
    QNetworkRequest request;
    request.setUrl(url);
    url.addQueryItem("api_token",m_apiToken);
    QNetworkReply *reply;
    connect(reply=_nam.post(request,url.encodedQuery()),SIGNAL(finished()),this,SLOT(parseAuthResponse()));
    reply->ignoreSslErrors();
}

QNetworkReply *TogglConnector::validateReply()
{
    QNetworkReply *reply = qobject_cast<QNetworkReply*>(sender());
    //qDebug() << reply->readAll();
    if(!reply)
        return reportError(GeneralError,tr("Slot connected to non-QNR signal")),(QNetworkReply*)0;
    if(reply->error())
        return reportError(NetworkError,reply->errorString()+"\nError was: "+QString().append(reply->readAll())),(QNetworkReply*)0;
    else if(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()>=300)
        return reportError(NetworkError,tr("Invalid Status Code (%1): %2").arg(QString::number(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt()),QString().append(reply->readAll())))
                ,(QNetworkReply*)0;
    return reply;
}

QVariant TogglConnector::parseResponse()
{
    QNetworkReply *reply = validateReply();
    if(!reply) return QVariant();
    bool ok;
    QJson::Parser parser;
    QVariant result = parser.parse(reply->readAll(),&ok).toMap()["data"];
    if(m_uploadedObjects.contains(reply)) {
        TogglObject *object = m_uploadedObjects.value(reply);
        object->m_id=(result.toMap()["id"]).toInt();
        addGenericObjectToMap(object);
        m_uploadedObjects.remove(reply);
    }
    if(!ok)
        return reportError(ParserError,tr("Parsing failed: %1").arg(parser.errorString())),QVariant();
    return result;
}

void TogglConnector::authenticate(QNetworkReply *reply, QAuthenticator *auth)
{
    Q_UNUSED(reply);
    auth->setUser(m_apiToken);
    auth->setPassword("api_token");
}

void TogglConnector::parseAuthResponse()
{
    for(int i = 0; i < 7;++i)
        queryObject(0,static_cast<ObjectType>(i));
}

void TogglConnector::loadProjects()
{
    connect(query(QUrl(urls[ProjectsUrl])),SIGNAL(finished()),SLOT(parseProjects()));
}

#define parseObjectsStub(ObjectType,listMethod,singleMethod,signal) void TogglConnector::listMethod() \
{                                                                                               \
    m_pendingQueries[ObjectType]->replace(0,-2);                                                \
    listMethod(parseResponse());                                                                \
}                                                                                               \
    void TogglConnector::listMethod(const QVariant &data)                                       \
{                                                                                               \
    const QVariantList list = data.toList();                                                    \
    foreach(const QVariant v, list)                                                             \
    singleMethod(v);                                                                            \
    emit signal();                                                                              \
}

parseObjectsStub(Project,parseProjects,parseProject,projectsLoaded)
parseObjectsStub(Client,parseClients,parseClient,clientsLoaded)
parseObjectsStub(Task,parseTasks,parseTask,tasksLoaded)
parseObjectsStub(TimeEntry,parseTimeEntries,parseTimeEntry,timeEntriesLoaded)
parseObjectsStub(Workspace,parseWorkspaces,parseWorkspace,workspacesLoaded)

void TogglConnector::parseUsers()
{
    parseUsers(parseResponse());
}
void TogglConnector::parseUsers(const QVariant &data)
{
    parseUser(data);
}

#define hasProperty(name)     (v.contains(name)&&mo->indexOfProperty(name)>0)
#define writeProperty(function,name) object->setProperty(name,function(v[name]));
#define relatedObject(function,name) if((v.contains(name)&&(i=mo->indexOfProperty(name))>0)) mo->property(i).write(object,QVariant::fromValue(function(v[name])))

void TogglConnector::setPropertries(const QVariantMap &v, TogglObject *object)
{
    int i;
    QJson::QObjectHelper::qvariant2qobject(v,object);
    const QMetaObject *mo = object->metaObject();
    relatedObject(parseProject,"project");
    relatedObject(parseWorkspace,"workspace");
    relatedObject(parseClient,"client");
    relatedObject(parseTask,"task");
    relatedObject(parseTimeEntry,"time_entry");
    relatedObject(parseUser,"user");
}

template < typename T > T *TogglConnector::parseGeneric(const QVariant &v, QMap<int,T*> &objectMap, void (TogglConnector::*queryFunction)(int),bool &isStub, bool &isUpdate, QVariantMap &map)
{
    map = v.toMap();
    int id = map["id"].toInt();
    isUpdate = objectMap.contains(id);
    isStub = map.count()<(T::staticMetaObject.propertyCount()-3);//additional QT properties are objectName isNew and isUnsaved
    T *c;
    if(isUpdate) {
        c=objectMap.value(id);
        if(!isStub&&c->hasPendingChanges()) {
            reportError(GeneralError,"Conflict in Data");
            return 0;
        }
/*
        else
            return c;
*/
    } else {
        c=new T(this);
        c->m_id = id;
        objectMap.insert(id,c);
    }
    setPropertries(map,c);
    c->m_new = false;
    c->m_unsaved = false;
    if(isStub) { (this->*queryFunction)(id); }
    else c->emitSaved();

    return c;
}

TogglProject *TogglConnector::parseProject(const QVariant &v)
{
    QVariantMap map;
    bool isStub;
    bool isUpdate;
    TogglProject *c=parseGeneric<TogglProject>(v,m_projects,&TogglConnector::queryProject,isStub,isUpdate,map);

    if(!isStub) {
        c->setBillingMethod(map["is_fixed_fee"].toBool()?TogglProject::FixedFee:TogglProject::HourlyRate);
        c->setEstimationMethod(map["automatically_calculate_estimated_workhours"].toBool()?TogglProject::Auto:TogglProject::FixedHours);
    }
    return c;
}

TogglClient    *TogglConnector::parseClient(const QVariant &v)
{
    return parseGeneric<TogglClient>(v,m_clients,&TogglConnector::queryClient);
}

TogglTask      *TogglConnector::parseTask(const QVariant &v)
{
    return parseGeneric<TogglTask>(v,m_tasks,&TogglConnector::queryTask);
}

TogglTimeEntry *TogglConnector::parseTimeEntry(const QVariant &v)
{
    return parseGeneric<TogglTimeEntry>(v,m_timeEntries,&TogglConnector::queryTimeEntry);
}

TogglWorkspace *TogglConnector::parseWorkspace(const QVariant &v)
{
    return parseGeneric<TogglWorkspace>(v,m_workspaces,&TogglConnector::queryWorkspace);
}

TogglUser      *TogglConnector::parseUser(const QVariant &v)
{
    return parseGeneric<TogglUser>(v,m_users,&TogglConnector::queryUser);
}

#define queryObjectStub(Object,Url,parser) void TogglConnector::query##Object(int id)   \
{                                                                                       \
    int first = m_pendingQueries[Object]->first();                                      \
    switch(first) {                                                                     \
        case -2: return; /*Query All*/                                                  \
        case -1: break; /*Query Some (not supported)*/                                  \
        default:                                                                        \
        case 0: connect(query(QUrl(urls[Url])),SIGNAL(finished()),SLOT(parser())); /*No Ongoing Queries; Query All*/ \
        m_pendingQueries[Object]->replace(0,-2);                                        \
        break;                                                                          \
    }                                                                                   \
}

queryObjectStub(Project,ProjectsUrl,parseProjects)
queryObjectStub(Client,ClientsUrl,parseClients)
queryObjectStub(Task,TasksUrl,parseTasks)
queryObjectStub(TimeEntry,TimeEntriesUrl,parseTimeEntries)
queryObjectStub(Workspace,WorkspacesUrl,parseWorkspaces)
queryObjectStub(User,CurrentUserUrl,parseUsers)

void TogglConnector::queryObject(int id, TogglConnector::ObjectType type)
{
    switch(type)
    {
    case Project: return queryProject(id);
    case Client: return queryClient(id);
    case Task: return queryTask(id);
    case TimeEntry: return queryTimeEntry(id);
    case Workspace: return queryWorkspace(id);
    case User: return queryUser(id);
    default: return;
    }
}

QMap<int,TogglProject*> TogglConnector::projects()
{
    return m_projects;
}

QMap<int,TogglClient*> TogglConnector::clients()
{
    return m_clients;
}

QMap<int,TogglTask*> TogglConnector::tasks()
{
    return m_tasks;
}

QMap<int,TogglTimeEntry*> TogglConnector::timeEntries()
{
    return m_timeEntries;
}

QMap<int,TogglWorkspace*> TogglConnector::worspaces()
{
    return m_workspaces;
}

QMap<int,TogglUser*> TogglConnector::users()
{
    return m_users;
}

QVariantMap TogglConnector::serializeObjectIdOnly(TogglObject *object)
{
    QVariantMap map;
    if(object!=0) map.insert("id",object->id());
    else return QVariant().toMap();
    return map;
}

template <typename T> bool TogglConnector::serializeSpecial(const QString &propertyName, QVariantMap &map, const QMetaObject *mo, TogglObject* object, const QStringList &ignores)
{
    int i = mo->indexOfProperty(propertyName.toAscii().data());
    if(i<0)
        return true;
    QVariant v = mo->property(i).read(object);
    if(!v.canConvert<T*>())
        return true;
    T* Tobject = v.value<T*>();
    if(Tobject == 0)
        return true;
    if(Tobject->isNew()) {
        Tobject->save();
        QObject::connect(Tobject,SIGNAL(saved()),object,SLOT(save()));
        return false;
    }
    if(!ignores.contains(propertyName))
        map.insert(propertyName,serializeObjectIdOnly(Tobject));
    return true;
}

#define serializeSpecialMacro(propertyName,className) (serializeSpecial<className>(propertyName,map,mo,object,additionalIgnores))

QVariantMap TogglConnector::serializeObject(TogglObject *object, const QStringList &additionalIgnores)
{
    static const QStringList ignoredProperties(QStringList()<<QLatin1String("objectName")<<
                                               QLatin1String("workspace")<<QLatin1String("project")<<
                                               QLatin1String("client")<<QLatin1String("task")<<
                                               QLatin1String("user")<<QLatin1String("isNew")<<
                                               QLatin1String("unsaved"));
    QVariantMap map = QJson::QObjectHelper::qobject2qvariant(object, ignoredProperties+additionalIgnores);
    const QMetaObject *mo = object->metaObject();
    if(serializeSpecialMacro("workspace",TogglWorkspace)&&
    serializeSpecialMacro("project",TogglProject)&&
    serializeSpecialMacro("client",TogglClient)&&
    serializeSpecialMacro("task",TogglTask)&&
    serializeSpecialMacro("user",TogglUser)&&
    serializeSpecialMacro("time_entry",TogglTimeEntry)) return map;
    else
        return QVariantMap();
}
#define debug
void TogglConnector::update(const QUrl &url, const QVariantMap &map, const char *slot,TogglObject *object)
{
    if(map.isEmpty())
        return;
    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader,"application/json");
    QJson::Serializer serializer;
    QNetworkReply *reply;
    connect(object->isNew()?reply=_nam.post(request,serializer.serialize(map)):
                  _nam.put(request,serializer.serialize(map)),SIGNAL(finished()),slot);
    object->m_unsaved = false;
    if(object->isNew()) m_uploadedObjects.insert(reply,object);
}

inline QUrl createUrl(urlPos newUrl, urlPos updateUrl, TogglObject *object)
{
    return object->isNew()?QUrl(urls[newUrl]):
                                QUrl(QString(urls[updateUrl]).arg(object->id()));
}

inline QVariantMap TogglConnector::createMap(QString objectName,TogglObject *object,QStringList ignores)
{
    QVariantMap map;
    QVariantMap objectMap=serializeObject(object,object->isNew()?ignores<<"id":ignores);
    if(objectMap.isEmpty())
        return QVariantMap();
    map.insert(objectName,objectMap);
    return map;
}

void TogglConnector::uploadProject(TogglProject *project)
{
    if(!project->hasPendingChanges())
        return;
    QVariantMap map;
    QStringList ignores;
    ignores << "billingMethod" << "estimationMethod" << "client_project_name";
    QVariantMap projectMap=serializeObject(project,project->isNew()?ignores<<"id":ignores);
    if(projectMap.isEmpty())
        return;
    projectMap.insert("automatically_calculate_estimated_workhours",project->estimationMethod()==TogglProject::Auto);
    projectMap.insert("is_fixed_fee",project->billingMethod()==TogglProject::FixedFee);
    map.insert("project",projectMap);
    update(createUrl(ProjectsUrl,ProjectDetailUrl,project),map,SLOT(parseProject()),project);
}


void TogglConnector::uploadClient(TogglClient *object)
{
    if(!object->hasPendingChanges())
        return;
    update(createUrl(ClientsUrl,ClientDetailUrl,object),
           createMap("client",object),SLOT(parseClient()),object);
}

void TogglConnector::uploadTask(TogglTask *task)
{
    if(!task->hasPendingChanges())
        return;
    update(createUrl(TasksUrl,TaskDetailUrl,task),
           createMap("client",task),SLOT(parseTask()),task);
}

void TogglConnector::uploadTimeEntry(TogglTimeEntry *entry)
{
    if(!entry->hasPendingChanges())
        return;
    update(createUrl(TimeEntriesUrl,TimeEntryDetailUrl,entry),
           createMap("time_entry",entry),SLOT(parseTimeEntry()),entry);
}

void TogglConnector::deleteObject(urlPos pos, TogglObject *object, const char *slot)
{
    QNetworkRequest request;
    request.setUrl(QUrl(QString(urls[pos]).arg(object->id())));
    connect(_nam.deleteResource(request),SIGNAL(finished()),slot);

}

void TogglConnector::deleteProject(TogglProject *project)
{
    if(project) deleteObject(ProjectDetailUrl,project,SLOT(completeProjectDeletion()));
}

void TogglConnector::deleteClient(TogglClient *client)
{
    if(client) deleteObject(ClientDetailUrl,client,SLOT(completeClientDeletion()));
}

void TogglConnector::deleteTask(TogglTask *task)
{
    if(task) deleteObject(TaskDetailUrl,task,SLOT(completeTaskDeletion()));
}

void TogglConnector::deleteTimeEntry(TogglTimeEntry *entry)
{
    if(entry) deleteObject(TimeEntryDetailUrl,entry,SLOT(completeTimeEntryDeletion()));
}

template <typename T > void TogglConnector::completeObjectDeletion(QMap<int,T*>* list)
{
    QNetworkReply *reply = validateReply();
    if(!reply)
        return;
    QString url = reply->url().toString();
    QRegExp rx = QRegExp("\\/([0-9]+)\\.json$");
    rx.indexIn(url);
    int id = rx.cap(1).toInt();
    list->remove(id);
}

void TogglConnector::completeProjectDeletion()
{
    completeObjectDeletion(&m_projects);
}

void TogglConnector::completeClientDeletion()
{
    completeObjectDeletion(&m_clients);
}

void TogglConnector::completeTaskDeletion()
{
    completeObjectDeletion(&m_tasks);
}

void TogglConnector::completeTimeEntryDeletion()
{
    completeObjectDeletion(&m_timeEntries);
}

void TogglConnector::parseProject()
{
    parseProject(parseResponse());
}

void TogglConnector::parseClient()
{
    parseClient(parseResponse());
}

void TogglConnector::parseTask()
{
    parseTask(parseResponse());
}

void TogglConnector::parseTimeEntry()
{
    parseTimeEntry(parseResponse());
}

void TogglConnector::addObjectToMap(TogglProject *project)
{
    if(project==0) return;
    m_projects.insert(project->id(),project);
}

void TogglConnector::addObjectToMap(TogglClient *client)
{
    if(client==0) return;
    m_clients.insert(client->id(),client);
}

void TogglConnector::addObjectToMap(TogglWorkspace *workspace)
{
    if(workspace==0) return;
    m_workspaces.insert(workspace->id(),workspace);
}

void TogglConnector::addObjectToMap(TogglTask *task)
{
    if(task==0) return;
    m_tasks.insert(task->id(),task);
}

void TogglConnector::addObjectToMap(TogglTimeEntry *timeEntry)
{
    if(timeEntry==0) return;
    m_timeEntries.insert(timeEntry->id(),timeEntry);
}

void TogglConnector::addObjectToMap(TogglUser *user)
{
    if(user==0) return;
    m_users.insert(user->id(),user);
}

void TogglConnector::addGenericObjectToMap(TogglObject *object)
{
    addObjectToMap(qobject_cast<TogglProject*>(object));
    addObjectToMap(qobject_cast<TogglWorkspace*>(object));
    addObjectToMap(qobject_cast<TogglClient*>(object));
    addObjectToMap(qobject_cast<TogglTask*>(object));
    addObjectToMap(qobject_cast<TogglTimeEntry*>(object));
    addObjectToMap(qobject_cast<TogglUser*>(object));
}

QList<TogglProject*> TogglConnector::findProjectByName(QString name)
{
    QList<TogglProject*> ret;
    foreach(TogglProject *project, m_projects)
        if(project->name() == name) ret << project;
    return ret;
}
}
