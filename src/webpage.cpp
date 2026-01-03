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
    // This should rarely be called since WebView::createWindow handles most cases
    // If it is called, we just return nullptr or create a new page
    return nullptr;
}
