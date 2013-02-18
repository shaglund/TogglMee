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
#ifndef TOGGL_TOGGLWORKSPACE_H
#define TOGGL_TOGGLWORKSPACE_H

#include "togglobject.h"

namespace Toggl {

class TOGGLCLIENTSHARED_EXPORT TogglWorkspace : public TogglObject
{
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged)

public:
    TogglWorkspace(TogglConnector *c);
    QString name();

public slots:
    void save();
    void setName(QString name);

signals:
    void nameChanged(QString name);

private:
    QString m_name;
};

} // namespace Toggl

#endif // TOGGL_TOGGLWORKSPACE_H
