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
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY busyChanged)
    Q_PROPERTY(QString userName READ userName WRITE setUserName NOTIFY userNameChanged)

public:
    explicit TogglLoader(QObject *parent = 0);
    bool hasApiKey() const;
    bool isBusy() const;
    QAbstractListModel* timeentryModel();
    void setContext(QDeclarativeContext *ctxt);
    QString userName() const;
    
signals:
    void apiKeyChanged(bool avail);
    void busyChanged(bool busy);
    void timeentryModelChanged(QAbstractListModel* model);
    void userNameChanged(const QString& name);
    
public slots:
    void setApiKey(const QString& key);
    void setBusy(bool busy);
    void setUserName(const QString& name);

private slots:
    void trySetUsername();

private:
    void setupTogglUi();
    QByteArray m_apiKey;
    QString m_firstPage;
    TogglUI *m_ui;
    TogglTimeEntryModel *m_timeentrymodel;
    QDeclarativeContext *m_ctxt;
    bool m_busy;
    QString m_username;
    
};

#endif // TOGGLLOADER_H
