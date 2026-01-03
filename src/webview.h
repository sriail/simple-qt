#ifndef WEBVIEW_H
#define WEBVIEW_H

#include <QWebEngineView>

class WebPage;

class WebView : public QWebEngineView
{
    Q_OBJECT

public:
    WebView(QWidget *parent = nullptr);
    
    void setPage(WebPage *page);

protected:
    QWebEngineView *createWindow(QWebEnginePage::WebWindowType type) override;

private:
    void createWebActions();

private:
    WebPage *m_page = nullptr;
};

#endif // WEBVIEW_H
