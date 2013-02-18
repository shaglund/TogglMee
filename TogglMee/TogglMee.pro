# Add more folders to ship with the application, here
folder_01.source = qml/TogglMee
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE5756654

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components
QT += network

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    togglui.cpp \
    toggltimeentrymodel.cpp \
    togglloader.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../TogglClient/release/ -lTogglClient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../TogglClient/debug/ -lTogglClient
else:symbian: LIBS += -lTogglClient
else:unix: LIBS += -L$$OUT_PWD/../TogglClient/ -lTogglClient

INCLUDEPATH += $$PWD/../TogglClient
DEPENDPATH += $$PWD/../TogglClient

HEADERS += \
    togglui.h \
    toggltimeentrymodel.h \
    togglloader.h

OTHER_FILES += \
    qml/TogglMee/Header.qml
