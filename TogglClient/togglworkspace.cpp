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
 * along with this library; see the file LICENSE.LGPL.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#include "togglconnector.h"
#include "togglworkspace.h"

namespace Toggl {

TogglWorkspace::TogglWorkspace(TogglConnector *c) : TogglObject(c)
{}

QString TogglWorkspace::name()
{
    return m_name;
}

void TogglWorkspace::setName(QString name)
{
    if(m_name!=name) {
        emit nameChanged(m_name = name);
        touch();
    }
}

void TogglWorkspace::save()
{}


} // namespace Toggl
