#  This file is part of of the TogglClient library
#
#  Copyright (C) 2011 Keno Fischer <keno.fischer@gmx.de>
#
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License version 2.1, as published by the Free Software Foundation.
#
# This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#
#  You should have received a copy of the GNU Lesser General Public License
#  along with this library; see the file COPYING.LIB.  If not, write to
#  the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
#  Boston, MA 02110-1301, USA.

QT       += network

QT       -= gui

TARGET = TogglClient
TEMPLATE = lib

DEFINES += TOGGLCLIENT_LIBRARY

SOURCES += \
    togglproject.cpp \
    togglobject.cpp \
    togglconnector.cpp \
    toggltimeentry.cpp \
    togglworkspace.cpp \
    togglclient.cpp \
    toggltask.cpp \
    toggluser.cpp

HEADERS +=\
        TogglClient_global.h \
    togglproject.h \
    togglobject.h \
    togglconnector.h \
    toggltimeentry.h \
    togglworkspace.h \
    togglclient.h \
    toggltask.h \
    toggluser.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE359E9C7
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = TogglClient.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

mac:QMAKE_LFLAGS += -F$$OUT_PWD/../QJson/lib
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../QJson/src/release/ -lqjson
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../QJson/src/debug/ -lqjson
else:mac: LIBS += -F$$OUT_PWD/../QJson/lib/ -framework qjson
else:symbian: LIBS += -lqjson
else:unix: LIBS += -L$$OUT_PWD/../QJson/lib/ -lqjson

INCLUDEPATH += $$PWD/../QJson/include
DEPENDPATH += $$PWD/../QJson/src
