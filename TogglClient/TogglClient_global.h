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

#ifndef TOGGLCLIENT_GLOBAL_H
#define TOGGLCLIENT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOGGLCLIENT_LIBRARY)
#  define TOGGLCLIENTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOGGLCLIENTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TOGGLCLIENT_GLOBAL_H
