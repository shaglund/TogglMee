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
#include "togglconnector.h"
#include "togglobject.h"

namespace Toggl {

TogglObject::TogglObject(TogglConnector *c) : QObject(c), m_new(true), m_unsaved(true)
{

}

int TogglObject::id() const
{
    return m_id;
}

bool TogglObject::isNew() const
{
    return m_new;
}
bool TogglObject::hasPendingChanges() const
{
    return m_unsaved;
}

void TogglObject::touch()
{
    m_unsaved = true;
}

void TogglObject::emitSaved()
{
    emit saved();
}

} // namespace Toggl
