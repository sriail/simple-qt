#include "browser.h"
#include "browserwindow.h"
#include <QApplication>
#include <QWebEngineProfile>
#include <QWebEngineSettings>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("SimpleBrowser");
    QCoreApplication::setApplicationName("Simple Browser");
    QCoreApplication::setApplicationVersion("1.0");

    QApplication app(argc, argv);
    
    // Configure web engine settings
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(
        QWebEngineSettings::PluginsEnabled, true);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(
        QWebEngineSettings::JavascriptEnabled, true);
    QWebEngineProfile::defaultProfile()->settings()->setAttribute(
        QWebEngineSettings::LocalStorageEnabled, true);
    
    Browser::instance().createWindow();
    
    return app.exec();
}
