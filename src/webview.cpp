#include "webview.h"
#include <QWebEnginePage>

WebView::WebView(QWidget *parent)
    : QWebEngineView(parent)
{
}

QWebEngineView *WebView::createWindow(QWebEnginePage::WebWindowType type)
{
    // Create a new tab when requested
    WebView *newView = new WebView();
    emit createNewTab(QUrl());
    return newView;
}
