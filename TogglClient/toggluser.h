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
#ifndef TOGGL_TOGGLUSER_H
#define TOGGL_TOGGLUSER_H

#include <QObject>

#include <QLocale>

#include "togglobject.h"

namespace Toggl {

class TogglWorkspace;

class TOGGLCLIENTSHARED_EXPORT TogglUser : public TogglObject
{
    Q_OBJECT
    Q_PROPERTY(QString fullname READ fullName WRITE setFullName NOTIFY fullNameChanged)
    Q_PROPERTY(QLocale language READ language WRITE setLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString api_token READ apiToken WRITE setApiToken NOTIFY apiTokenChanged)
    Q_PROPERTY(Toggl::TogglWorkspace *default_workspace READ defaultWorkspace WRITE setDefaultWorkspace NOTIFY defaultWorkspaceChanged)
    Q_PROPERTY(QString email READ email WRITE setEmail NOTIFY emailChanged)
    Q_PROPERTY(bool new_time_entries_start_automatically READ autoStartNewEntries WRITE setAutoStartNewEntries NOTIFY autoStartNewEntriesChanged)
    Q_PROPERTY(QString jquery_timeofday_format READ jQueryTimeOfDayFormat WRITE setJQueryTimeOfDayFormat NOTIFY jQueryTimeOfDayFormatChanged)
    Q_PROPERTY(QString timeofday_format READ timeOfDayFormat WRITE setTimeOfDayFormat NOTIFY timeOfDayFormatChanged)
    Q_PROPERTY(QString jquery_data_format READ jQueryDateFormat WRITE setJQueryDateFormat NOTIFY jQueryDateFormatChanged)
    Q_PROPERTY(QString date_format READ dateFormat WRITE setDateFormat NOTIFY dateFormatChanged)
    Q_PROPERTY(int time_entry_retention_time READ timeEntryRetentionTime WRITE setTimeEntryRetentionTime NOTIFY timeEntryRetentionTimeChanged)
    Q_PROPERTY(int beginning_of_week READ weekStartsOn WRITE setWeekStartsOn NOTIFY weekStartOnChanged)
public:
    TogglUser(TogglConnector *c);
    QString fullName() const;
    QLocale language() const;
    QString apiToken() const;
    TogglWorkspace *defaultWorkspace() const;
    QString email() const;
    bool autoStartNewEntries() const;
    QString jQueryTimeOfDayFormat() const;
    QString timeOfDayFormat() const;
    QString jQueryDateFormat() const;
    QString dateFormat() const;
    int timeEntryRetentionTime() const;
    int weekStartsOn() const;
public slots:
    void save();
    void setFullName(QString fullName);
    void setLanguage(QLocale language);
    void setApiToken(QString token);
    void setDefaultWorkspace(TogglWorkspace *workspace);
    void setEmail(QString email);
    void setAutoStartNewEntries(bool entries);
    void setJQueryTimeOfDayFormat(QString format);
    void setTimeOfDayFormat(QString format);
    void setJQueryDateFormat(QString format);
    void setDateFormat(QString format);
    void setTimeEntryRetentionTime(int days);
    void setWeekStartsOn(int weekday);
signals:
    void fullNameChanged(QString newName);
    void languageChanged(QLocale newLanguage);
    void apiTokenChanged(QString newApiToken);
    void defaultWorkspaceChanged(TogglWorkspace *newDefaultWorkspace);
    void emailChanged(QString newEmail);
    void autoStartNewEntriesChanged(bool autoStart);
    void jQueryTimeOfDayFormatChanged(QString newJQueryToDFormat);
    void timeOfDayFormatChanged(QString newToDFormat);
    void jQueryDateFormatChanged(QString newJQDateFormat);
    void dateFormatChanged(QString newDateFormat);
    void timeEntryRetentionTimeChanged(int newRetentionTime);
    void weekStartOnChanged(int newStartOfTheWeek);

private:
    QString m_fullName;
    QLocale m_language;
    QString m_apiToken;
    TogglWorkspace *m_defaultWorkspace;
    QString m_email;
    bool m_autoStartNewEntries;
    QString m_jQueryTimeOfDayFormat;
    QString m_timeOfDayFormat;
    QString m_jQueryDateFormat;
    QString m_dateFormat;
    int m_timeEntryRetentionTime;
    int m_weekStartsOn;
};

} // namespace Toggl

#endif // TOGGL_TOGGLUSER_H
