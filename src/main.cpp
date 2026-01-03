#include <QApplication>
#include <QtWebEngineWidgets>
#include "browser.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // Set application information
    QApplication::setApplicationName("Simple Browser");
    QApplication::setApplicationVersion("1.0.0");
    QApplication::setOrganizationName("Simple Qt");

    // Create and show browser window
    Browser browser;
    browser.show();

    return app.exec();
}
