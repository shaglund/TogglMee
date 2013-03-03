#include "togglloader.h"
#include <QDeclarativeContext>
#include <QSettings>
#include <QDebug>
#include "qmlapplicationviewer.h"

TogglLoader::TogglLoader(QObject *parent) :
    QObject(parent), m_msgModel(new MessageModel()), m_ui(NULL), m_timeentrymodel(NULL), m_ctxt(NULL), m_busy(false)
{
    QSettings settings("haglund.se", "TogglMee");
    m_apiKey = settings.value("ApiKey").toByteArray();
    qDebug() << "ApiKey found in " << settings.fileName();

    if(m_apiKey.length()) {
        emit apiKeyChanged(true);
        setupTogglUi();
    } else {
        emit apiKeyChanged(false);
    }
}

bool TogglLoader::hasApiKey() const
{
    return m_apiKey.length()?true:false;
}

bool TogglLoader::isBusy() const
{
    return m_busy;
}

QString TogglLoader::userName() const
{
    return m_username;
}

void TogglLoader::setUserName(const QString &name)
{
    emit userNameChanged(m_username = name);
}

void TogglLoader::setBusy(bool busy)
{
    emit busyChanged(m_busy = busy);
}

QAbstractListModel *TogglLoader::timeentryModel()
{
    return m_timeentrymodel;
    // return reinterpret_cast<QAbstractListModel*>(m_timeentrymodel);
}

void TogglLoader::setApiKey(const QString &key)
{
    m_apiKey = key.toLocal8Bit();
    QSettings settings("haglund.se", "TogglMee");
    settings.setValue("ApiKey", key);
    if(key.size() == 0 && m_ui)  {
        delete m_ui;
        emit apiKeyChanged(false);
        m_timeentrymodel->deleteLater();
    } else {
        setupTogglUi();
    }
}

void TogglLoader::trySetUsername()
{
    QMap<int, Toggl::TogglUser*> users = m_ui->togglConnector()->users();
    if(users.isEmpty()) {
        QTimer::singleShot(250, this, SLOT(trySetUsername()));
    } else {
        Toggl::TogglUser *u = users.values().first();
        setUserName(u->fullName());
    }
}

void TogglLoader::setContext(QDeclarativeContext *ctxt)
{
    m_ctxt = ctxt;
    if(m_timeentrymodel) {// Already initialized -> set model
        m_ctxt->setContextProperty("timeentryModel", m_timeentrymodel);
    }
}

void TogglLoader::setupTogglUi()
{
    m_ui = new TogglUI(m_apiKey);
    m_timeentrymodel = new TogglTimeEntryModel(m_ui);
    connect(m_timeentrymodel, SIGNAL(togglIsBusy(bool)), this, SLOT(setBusy(bool)));
    if(m_ctxt) {
        m_ctxt->setContextProperty("timeentryModel", m_timeentrymodel);        
    }
    emit timeentryModelChanged(reinterpret_cast<QAbstractListModel*>(m_timeentrymodel));
    QObject::connect(m_ui, SIGNAL(timeEntriesLoaded()), m_timeentrymodel, SLOT(setTimeEntries()));
    setBusy(true);
    m_ui->init();
    emit apiKeyChanged(true);
    if(m_username.isEmpty())
        QTimer::singleShot(250, this, SLOT(trySetUsername()));
    connect(m_ui->togglConnector(), SIGNAL(message(QString)), this, SLOT(addMessage(QString)));
}

void TogglLoader::addMessage(const QString &msg)
{
    fprintf(stderr, "%s\n", msg.toAscii().constData());
    m_msgModel->addString(msg);
}
