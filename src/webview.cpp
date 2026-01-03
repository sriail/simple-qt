#include "webview.h"
#include "webpage.h"
#include "browserwindow.h"
#include "tabwidget.h"
#include <QWebEngineProfile>
#include <QContextMenuEvent>

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
    m_page = new WebPage(QWebEngineProfile::defaultProfile(), this);
    QWebEngineView::setPage(m_page);
}

void WebView::setPage(WebPage *page)
{
    if (m_page)
        m_page->deleteLater();
    m_page = page;
    QWebEngineView::setPage(page);
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    Q_UNUSED(type);
    
    BrowserWindow *mainWindow = qobject_cast<BrowserWindow*>(window());
    if (!mainWindow)
        return nullptr;

    return mainWindow->tabWidget()->createTab();
}
