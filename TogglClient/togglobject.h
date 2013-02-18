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

#ifndef TOGGL_TOGGLOBJECT_H
#define TOGGL_TOGGLOBJECT_H

#include <QObject>
#include "TogglClient_global.h"

namespace Toggl {

class TogglConnector;

class TOGGLCLIENTSHARED_EXPORT TogglObject : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id READ id)
    Q_PROPERTY(int isNew READ isNew NOTIFY saved)
    Q_PROPERTY(int unsaved READ hasPendingChanges NOTIFY saved)
public:
    TogglObject(TogglConnector *c);
    bool isNew() const;
    bool hasPendingChanges() const;
    int id() const;
public slots:
    virtual void save() = 0;
signals:
    void saved();
protected:
    void touch();
    TogglObject(bool isNew, bool isUnsaved, int id, QObject *parent=0);
private:
    TogglObject(const TogglObject&);
    void emitSaved();
    int m_id;
    int m_new;
    int m_unsaved;

    friend class TogglConnector;
};


} // namespace Toggl

#endif // TOGGL_TOGGLOBJECT_H
