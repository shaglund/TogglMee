#include "togglloader.h"
#include <QDeclarativeContext>
#include <QSettings>
#include <QDebug>
#include "qmlapplicationviewer.h"

TogglLoader::TogglLoader(QObject *parent) :
    QObject(parent), m_ui(NULL), m_timeentrymodel(NULL), m_ctxt(NULL)
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

bool TogglLoader::hasApiKey()
{
    return m_apiKey.length()?true:false;
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
    setupTogglUi();
}

void TogglLoader::setContext(QDeclarativeContext *ctxt)
{
    m_ctxt = ctxt;
    if(m_timeentrymodel) // Already initialized -> set model
        m_ctxt->setContextProperty("timeentryModel", m_timeentrymodel);
}

void TogglLoader::setupTogglUi()
{
    m_ui = new TogglUI(m_apiKey);
    m_timeentrymodel = new TogglTimeEntryModel(m_ui);
    if(m_ctxt)
        m_ctxt->setContextProperty("timeentryModel", m_timeentrymodel);
    emit timeentryModelChanged(reinterpret_cast<QAbstractListModel*>(m_timeentrymodel));
    QObject::connect(m_ui, SIGNAL(timeEntriesLoaded()), m_timeentrymodel, SLOT(setTimeEntries()));
    m_ui->init();
    emit apiKeyChanged(true);
}
