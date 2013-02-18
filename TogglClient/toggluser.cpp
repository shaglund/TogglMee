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
#include "toggluser.h"

namespace Toggl {


TogglUser::TogglUser(TogglConnector *c) : TogglObject(c)
{}

QString TogglUser::fullName() const
{
    return m_fullName;
}
QLocale TogglUser::language() const
{
    return m_language;
}
QString TogglUser::apiToken() const
{
    return m_apiToken;
}
TogglWorkspace *TogglUser::defaultWorkspace() const
{
    return m_defaultWorkspace;
}
QString TogglUser::email() const
{
    return m_email;
}
bool TogglUser::autoStartNewEntries() const
{
    return m_autoStartNewEntries;
}
QString TogglUser::jQueryTimeOfDayFormat() const
{
    return m_jQueryTimeOfDayFormat;
}
QString TogglUser::timeOfDayFormat() const
{
    return m_timeOfDayFormat;
}
QString TogglUser::jQueryDateFormat() const
{
    return m_jQueryDateFormat;
}
QString TogglUser::dateFormat() const
{
    return m_dateFormat;
}
int TogglUser::timeEntryRetentionTime() const
{
    return m_timeEntryRetentionTime;
}
int TogglUser::weekStartsOn() const
{
    return m_weekStartsOn;
}

void TogglUser::setFullName(QString fullName)
{
    if(m_fullName!=fullName) {
        emit fullNameChanged(m_fullName = fullName);
        touch();
    }

}

void TogglUser::setLanguage(QLocale language)
{
    if(m_language!=language) {
        emit languageChanged(m_language = language);
        touch();
    }

}

void TogglUser::setApiToken(QString token)
{
    if(m_apiToken!=token) {
        emit apiTokenChanged(m_apiToken = token);
        touch();
    }

}

void TogglUser::setDefaultWorkspace(TogglWorkspace *workspace)
{
    if(m_defaultWorkspace!=workspace) {
        emit defaultWorkspaceChanged(m_defaultWorkspace = workspace);
        touch();
    }

}

void TogglUser::setEmail(QString email)
{
    if(m_email!=email) {
        emit emailChanged(m_email = email);
        touch();
    }

}

void TogglUser::setAutoStartNewEntries(bool entries)
{
    if(m_autoStartNewEntries!=entries) {
        emit autoStartNewEntriesChanged(m_autoStartNewEntries = entries);
        touch();
    }

}

void TogglUser::setJQueryTimeOfDayFormat(QString format)
{
    if(m_jQueryTimeOfDayFormat!=format) {
        emit jQueryTimeOfDayFormatChanged(m_jQueryTimeOfDayFormat = format);
        touch();
    }

}

void TogglUser::setTimeOfDayFormat(QString format)
{
    if(m_timeOfDayFormat!=format) {
        emit timeOfDayFormatChanged(m_timeOfDayFormat = format);
        touch();
    }

}

void TogglUser::setJQueryDateFormat(QString format)
{
    if(m_jQueryDateFormat!=format) {
        emit jQueryDateFormatChanged(m_jQueryDateFormat = format);
        touch();
    }
}

void TogglUser::setDateFormat(QString format)
{
    if(m_dateFormat!=format) {
        emit dateFormatChanged(m_dateFormat = format);
        touch();
    }
}

void TogglUser::setTimeEntryRetentionTime(int days)
{
    if(m_timeEntryRetentionTime!=days) {
        emit timeEntryRetentionTimeChanged(m_timeEntryRetentionTime = days);
        touch();
    }

}

void TogglUser::setWeekStartsOn(int weekday)
{
    if(m_weekStartsOn!=weekday) {
        emit weekStartOnChanged(m_weekStartsOn = weekday);
        touch();
    }

}

void TogglUser::save()
{}


} // namespace Toggl
