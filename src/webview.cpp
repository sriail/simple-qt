#include "webview.h"
#include <QWebEnginePage>

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    // Emit signal to let the browser create a new tab
    // The browser will handle the new view creation
    emit createNewTab(QUrl());
    
    // Return nullptr as we handle tab creation through the signal
    // The browser will set up the new tab properly
    return nullptr;
}
