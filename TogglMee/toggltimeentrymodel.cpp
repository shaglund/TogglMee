#include "toggltimeentrymodel.h"
#include "togglproject.h"
#include <QDebug>

static const QString default_description = QString("What are you working on?");

TogglTimeEntryModel::TogglTimeEntryModel(TogglUI *toggl, QObject *parent) :
    QAbstractListModel(parent),
    m_timeentries(QList<Toggl::TogglTimeEntry*>()),
    m_current(NULL),
    m_toggl(toggl),
    m_togglText("Start"),
    m_togglDescription(default_description),
    m_togglDuration(),
    m_durationTimer(new QTimer(this))
{
    QHash<int, QByteArray> roles;
    roles[DescriptionRole] = "description";
    roles[ProjectRole] = "project";
    roles[DurationRole] = "duration";
    roles[DateRole] = "date";
    setRoleNames(roles);
    connect(m_durationTimer, SIGNAL(timeout()), this, SLOT(updateDuration()));
}

int TogglTimeEntryModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_timeentries.count();
}

static QString seconds_to_timestamp(int duration)
{
    QString res;
    int seconds = (int) (duration % 60);
    duration /= 60;
    int minutes = (int) (duration % 60);
    duration /= 60;
    int hours = (int) (duration % 24);
    if(!minutes && !hours)
        return res.sprintf("%d sec", seconds);
    return res.sprintf("%02d:%02d%s", hours?hours:minutes, hours?minutes:seconds, hours?"":" min");
}

QVariant TogglTimeEntryModel::data(const QModelIndex &index, int role) const
{
    if(index.row() < 0 || index.row() > m_timeentries.count())
        return QVariant();

    QDateTime current = QDateTime::currentDateTimeUtc();
    QDateTime start;
    int duration, day, today = current.date().day();
    int yesterday = current.addDays(-1).date().day();
    QString res;

    Toggl::TogglTimeEntry* entry = m_timeentries[index.row()];
    switch(role) {
    case DescriptionRole:
        return entry->description();
    case ProjectRole:
        return entry->project()->name();
    case DurationRole:
        return seconds_to_timestamp(entry->duration());
    case DateRole:
        start = entry->startDateTime();
        day = start.date().day();
        duration = m_dailyDurations[start.date()];
        if(day == today)
            res = "Today";
        else if(day == yesterday)
            res = "Yesterday";
        else
            res = start.date().toString();
        return res.append(" - ").append(seconds_to_timestamp(duration));
    default:
        return QVariant();
    }
}

QString TogglTimeEntryModel::togglText()
{
    return m_togglText;
}

QString TogglTimeEntryModel::togglDuration()
{
    return seconds_to_timestamp(m_togglDuration);
}

QString TogglTimeEntryModel::togglDescription()
{
    return m_togglDescription;
}

void TogglTimeEntryModel::setTogglText(const QString &s)
{
    if(m_togglText != s)
        emit togglTextChanged(m_togglText = s);
}

void TogglTimeEntryModel::setTogglDuration(const QString &s)
{
    emit togglDurationChanged(s);
}

void TogglTimeEntryModel::updateDuration()
{
    m_togglDuration++;
    setTogglDuration(seconds_to_timestamp(m_togglDuration));
}

void TogglTimeEntryModel::addTimeEntry(Toggl::TogglTimeEntry *t)
{
    if(t->duration() > 0) {
        beginInsertRows(QModelIndex(), m_timeentries.count(), m_timeentries.count());
        m_timeentries.insert(0, t);
        endInsertRows();
    } else {
        m_current = t;
    }
}

void TogglTimeEntryModel::currentSaved()
{
    if(m_current->duration() > 0) {
        Toggl::TogglTimeEntry *entry = m_current;
        m_current = NULL;
        beginInsertRows(QModelIndex(), 0, 0);
        m_timeentries.insert(0, entry);
        QDate d = entry->startDateTime().date();
        int current = m_dailyDurations[d];
        m_dailyDurations[d] = current + entry->duration();
        endInsertRows();
        m_current=NULL;
    }
}

void TogglTimeEntryModel::setTimeEntries()
{
    m_timeentries.clear();
    for(int i=0; i<m_toggl->time_entry_list().size(); i++) {
        Toggl::TogglTimeEntry *entry = m_toggl->time_entry_list().at(i);
        if(entry->duration() > 0) {
            m_timeentries.insert(i, entry);
            QDate d = entry->startDateTime().date();
            int current = m_dailyDurations[d];
            m_dailyDurations[d] = current + entry->duration();
        } else {
            m_current = entry;
            setTogglText("Stop");
            setTogglDuration(seconds_to_timestamp(m_togglDuration = QDateTime::currentDateTimeUtc().toTime_t() + entry->duration()));
            emit togglDescriptionChanged(m_togglDescription = entry->description());
            m_durationTimer->start(1000);
        }
    }
    reset();
}

void TogglTimeEntryModel::deleteTimeEntry(int index) {
    if(index < 0 || index > m_timeentries.size())
        return;
    Toggl::TogglTimeEntry *entry = m_timeentries[index];
    if(!entry)
        return;
    beginRemoveRows(QModelIndex(), index, index);
    m_timeentries.removeAt(index);
    QDate d = entry->startDateTime().date();
    int current = m_dailyDurations[d];
    m_dailyDurations[d] = current - entry->duration();
    m_toggl->togglConnector()->deleteTimeEntry(entry);
    endRemoveRows();
}

void TogglTimeEntryModel::continueTimeEntry(int index) {
    if(index < 0 || index > m_timeentries.size())
        return;
    Toggl::TogglTimeEntry *entry = m_timeentries[index];
    if(!entry)
        return;
    Toggl::TogglTimeEntry *new_entry = new Toggl::TogglTimeEntry(m_toggl->togglConnector());
    QDateTime now = QDateTime::currentDateTimeUtc();
    new_entry->setDescription(entry->description());
    new_entry->setProject(entry->project());
    new_entry->setBillable(entry->isBillable());
    new_entry->setWorkspace(entry->workspace());
    new_entry->setStart(now);
    new_entry->setDuration(0-now.toTime_t());
    connect(new_entry, SIGNAL(saved()), this, SLOT(currentSaved()));
    if(m_current) {
        m_current->setStop(now);
        m_current->setDuration(m_current->startDateTime().secsTo(now));
        m_current->save();
    }
    m_current=new_entry;
    setTogglText("Stop");
    setTogglDuration(seconds_to_timestamp(m_togglDuration=0));
    emit togglDescriptionChanged(m_togglDescription=new_entry->description());
    m_durationTimer->start(1000);
    new_entry->save();
}

void TogglTimeEntryModel::toggl(const QVariant &description) {
    QDateTime now = QDateTime::currentDateTimeUtc();
    if(!m_current) {        
        m_current = new Toggl::TogglTimeEntry(m_toggl->togglConnector());
        m_current->setDescription(description.toString());
        m_current->setStart(now);
        m_current->setDuration(0-now.toTime_t());
        m_current->setBillable(false);
        setTogglText("Stop");
        connect(m_current, SIGNAL(saved()), this, SLOT(currentSaved()));
        m_togglDuration=0;
        m_durationTimer->start(1000);
    } else {
        m_current->setStop(now);
        m_durationTimer->stop();
        setTogglDuration(seconds_to_timestamp(m_togglDuration=0));
        m_current->setDuration(m_current->startDateTime().secsTo(now));
        setTogglText("Start");
        emit togglDurationChanged(m_togglDescription=default_description);
    }
    m_current->setCreatedWith("TogglMee");
    QTimer::singleShot(20, m_current, SLOT(save()));
}
