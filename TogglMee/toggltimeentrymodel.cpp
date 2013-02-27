#include "toggltimeentrymodel.h"
#include "togglproject.h"
#include <QDebug>

static const QString default_description = QString("What are you working on?");

TogglTimeEntryModel::TogglTimeEntryModel(TogglUI *toggl, QObject *parent) :
    QAbstractListModel(parent),
    m_timeentries(QList<Toggl::TogglTimeEntry*>()),
    m_current(NULL),
    m_continue(NULL),
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
    if(!hours)
        return res.sprintf("%02d:%02d min", minutes, seconds);
    return res.sprintf("%02d:%02d:%02d", hours, minutes, seconds);
}

static int timestamp_to_seconds(const QString& time)
{
    int i, duration, ret=0;
    bool ok;
    const int multipliers[3] = {3600, 60, 1};
    QStringList parts = time.split(":");

    if(parts.size() > 3) return 0;
    int j=3-parts.size();
    for(i=0; i<parts.size(); i++) {
        duration = parts.at(i).toInt(&ok);
        if(!ok || duration < 0) return ret;
        duration *= multipliers[j++];
        ret += duration;
    }
    return ret;
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
        beginResetModel();
        m_timeentries.insert(0, entry);
        QDate d = entry->startDateTime().date();
        int current = m_dailyDurations[d];
        m_dailyDurations[d] = current + entry->duration();
        endResetModel();
        if(m_continue) {
            m_continue->save();
            m_current = m_continue;
            m_continue = NULL;
        } else
            emit togglIsBusy(false);
    } else
        emit togglIsBusy(false);
}

void TogglTimeEntryModel::setTimeEntries()
{
    m_timeentries.clear();
    m_dailyDurations.clear();
    for(int i=0; i<m_toggl->time_entry_list().size(); i++) {
        Toggl::TogglTimeEntry *entry = m_toggl->time_entry_list().at(i);
        if(entry->duration() > 0) {
            m_timeentries.prepend(entry);
            QDate d = entry->startDateTime().date();
            int current = m_dailyDurations[d];
            m_dailyDurations[d] = current + entry->duration();
        } else {
            m_current = entry;
            startTogglTimer(QDateTime::currentDateTimeUtc().toTime_t() + entry->duration());
            emit togglDescriptionChanged(m_togglDescription = entry->description());
        }
    }
    reset();
    emit togglIsBusy(false);
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

void TogglTimeEntryModel::startTogglTimer(int start_duration)
{
    setTogglText("Stop");
    setTogglDuration(seconds_to_timestamp(m_togglDuration=start_duration));
    m_durationTimer->start(1000);
}

void TogglTimeEntryModel::continueTimeEntry(int index) {
    if(index < 0 || index > m_timeentries.size())
        return;
    Toggl::TogglTimeEntry *entry = m_timeentries[index];
    if(!entry)
        return;
    QDateTime now = QDateTime::currentDateTimeUtc();
    m_continue = new Toggl::TogglTimeEntry(m_toggl->togglConnector());
    m_continue->setDescription(entry->description());
    m_continue->setProject(entry->project());
    m_continue->setBillable(entry->isBillable());
    m_continue->setWorkspace(entry->workspace());
    m_continue->setStart(now);
    m_continue->setDuration(0-now.toTime_t());
    connect(m_continue, SIGNAL(saved()), this, SLOT(currentSaved()));
    if(m_current) {
        m_current->setStop(now);        
        m_current->setDuration(now.toTime_t()+m_current->duration());
        m_current->save();
        emit togglIsBusy(true);
    } else {
        m_current=m_continue;
        m_continue=NULL;
        m_current->save();
        emit togglIsBusy(true);
    }
    startTogglTimer(0);
    emit togglDescriptionChanged(m_togglDescription=entry->description());
}

void TogglTimeEntryModel::toggl(const QVariant &description) {
    QDateTime now = QDateTime::currentDateTimeUtc();
    if(!m_current) {        
        m_current = new Toggl::TogglTimeEntry(m_toggl->togglConnector());
        m_current->setDescription(description.toString());
        m_current->setStart(now);
        m_current->setDuration(0-now.toTime_t());
        m_current->setBillable(false);
        connect(m_current, SIGNAL(saved()), this, SLOT(currentSaved()));
        startTogglTimer(0);
    } else {
        m_current->setStop(now);
        m_durationTimer->stop();
        setTogglDuration(seconds_to_timestamp(m_togglDuration=0));
        m_current->setDuration(now.toTime_t()+m_current->duration());
        setTogglText("Start");
        emit togglDurationChanged(m_togglDescription=default_description);
    }
    m_current->setCreatedWith("TogglMee");
    m_current->save();
    emit togglIsBusy(true);
}
