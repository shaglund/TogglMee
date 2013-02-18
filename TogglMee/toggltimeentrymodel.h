#ifndef TOGGLTIMEENTRYMODEL_H
#define TOGGLTIMEENTRYMODEL_H

#include <QAbstractListModel>
#include <QMap>
#include <QTimer>
#include <toggltimeentry.h>
#include <togglui.h>

class TogglTimeEntryModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString togglText READ togglText WRITE setTogglText NOTIFY togglTextChanged)
    Q_PROPERTY(QString togglDuration READ togglDuration WRITE setTogglDuration NOTIFY togglDurationChanged)
    Q_PROPERTY(QString togglDescription READ togglDescription NOTIFY togglDescriptionChanged)
public:
    explicit TogglTimeEntryModel(TogglUI *toggl, QObject *parent = 0);

    enum Roles { DescriptionRole = Qt::UserRole+1, ProjectRole = Qt::UserRole+2, DurationRole = Qt::UserRole+3, DateRole = Qt::UserRole+4 };

    int rowCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;    
    QString togglText();
    QString togglDuration();
    QString togglDescription();
    void setTogglText(const QString &s);
    void setTogglDuration(const QString &s);
    void addTimeEntry(Toggl::TogglTimeEntry *t);
    
signals:
    void togglTextChanged(const QString& text);
    void togglDurationChanged(const QString& text);
    void togglDescriptionChanged(const QString& text);
    
public slots:
    void setTimeEntries();
    void toggl(const QVariant& description);
    void currentSaved();
    void updateDuration();
    void deleteTimeEntry(int index);
    void continueTimeEntry(int index);

private:
    QList<Toggl::TogglTimeEntry*> m_timeentries;
    Toggl::TogglTimeEntry *m_current;
    QMap<QDate, long> m_dailyDurations;
    TogglUI *m_toggl;
    QString m_togglText;
    QString m_togglDescription;
    int m_togglDuration;
    QTimer *m_durationTimer;

};

#endif // TOGGLTIMEENTRYMODEL_H
