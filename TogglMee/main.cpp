#include <QtGui/QApplication>
#include <QDeclarativeContext>
#include <QSettings>
#include <QDebug>
#include <QObject>
#include "qmlapplicationviewer.h"
#include "togglloader.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    TogglLoader *loader = new TogglLoader();

    QmlApplicationViewer viewer;
    QDeclarativeContext *ctxt = viewer.rootContext();
    loader->setContext(ctxt);
    ctxt->setContextProperty("loader", loader);

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockPortrait);
    viewer.setMainQmlFile(QLatin1String("qml/TogglMee/main.qml"));
    viewer.showExpanded();

    return app->exec();
}
