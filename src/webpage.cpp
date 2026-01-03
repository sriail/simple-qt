#include "webpage.h"
#include "browserwindow.h"
#include "browser.h"
#include "tabwidget.h"
#include "webview.h"
#include <QWebEngineProfile>

WebPage::WebPage(QWebEngineProfile *profile, QObject *parent)
    : QWebEnginePage(profile, parent)
{
}

QWebEnginePage *WebPage::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type);
    
    BrowserWindow *mainWindow = qobject_cast<BrowserWindow*>(view()->window());
    if (!mainWindow)
        return nullptr;

    return mainWindow->tabWidget()->createTab()->page();
}
