#ifndef TOGGLLOADER_H
#define TOGGLLOADER_H

#include <QObject>
#include <QString>
#include <QDeclarativeContext>
#include "togglui.h"
#include "toggltimeentrymodel.h"

class TogglLoader : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QAbstractItemModel *timeentryModel READ timeentryModel NOTIFY timeentryModelChanged)
    Q_PROPERTY(bool hasApiKey READ hasApiKey NOTIFY apiKeyChanged)

public:
    explicit TogglLoader(QObject *parent = 0);
    bool hasApiKey();
    QAbstractListModel* timeentryModel();
    void setContext(QDeclarativeContext *ctxt);
    
signals:
    void apiKeyChanged(bool avail);
    void timeentryModelChanged(QAbstractListModel* model);
    
public slots:
    void setApiKey(const QString& key);

private:
    void setupTogglUi();
    QByteArray m_apiKey;
    QString m_firstPage;
    TogglUI *m_ui;
    TogglTimeEntryModel *m_timeentrymodel;
    QDeclarativeContext *m_ctxt;
    
};

#endif // TOGGLLOADER_H
