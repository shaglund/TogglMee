#ifndef TOGGLLOADER_H
#define TOGGLLOADER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QStringListModel>
#include <QDeclarativeContext>
#include "togglui.h"
#include "toggltimeentrymodel.h"

class MessageModel : public QAbstractListModel
{
public:
    explicit MessageModel(QObject *parent = 0) : QAbstractListModel(parent) { }
    explicit MessageModel(const QStringList &strings, QObject *parent = 0) : QAbstractListModel(parent), lst(strings) { }
    int rowCount(const QModelIndex &parent) const
    {
        if (parent.isValid()) return 0;
        return lst.count();
    }
    QVariant data(const QModelIndex &index, int role) const
    {
        if (index.row() < 0 || index.row() >= lst.size()) return QVariant();
        if (role == Qt::DisplayRole || role == Qt::EditRole) return lst.at(index.row());
        return QVariant();
    }
    void addString(const QString &str)
    {
        beginInsertRows(QModelIndex(), lst.count(), lst.count());
        lst << str;
        endInsertRows();
    }

private:
    QStringList lst;
};

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
    MessageModel *m_msgModel;

signals:
    void apiKeyChanged(bool avail);
    void busyChanged(bool busy);
    void timeentryModelChanged(QAbstractListModel* model);
    void userNameChanged(const QString& name);
    
public slots:
    void setApiKey(const QString& key);
    void setBusy(bool busy);
    void setUserName(const QString& name);
    void addMessage(const QString& msg);

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
    QStringList m_messages;
};

#endif // TOGGLLOADER_H
