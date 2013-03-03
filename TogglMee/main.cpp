#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include <QSettings>
#include <QDebug>
#include <QObject>
#include "qmlapplicationviewer.h"
#include "togglloader.h"

static TogglLoader *loader;

void togglMsgHandler(QtMsgType type, const char *msg)
{
    QString log;
    switch(type) {
    case QtDebugMsg:
        log = "Debug: ";
        break;
    case QtWarningMsg:
        log = "Warning: ";
        break;
    case QtCriticalMsg:
        log = "Critical: ";
        break;
    case QtFatalMsg:
        log = "Fatal";
        break;
    }
    log.append(msg);
    loader->addMessage(log);
}


Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    loader = new TogglLoader();
    qInstallMsgHandler(togglMsgHandler);
    QmlApplicationViewer viewer;
    QDeclarativeContext *ctxt = viewer.rootContext();
    loader->setContext(ctxt);
    ctxt->setContextProperty("loader", loader);
    ctxt->setContextProperty("messagesModel", loader->m_msgModel);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);
    viewer.setMainQmlFile(QLatin1String("qml/TogglMee/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
