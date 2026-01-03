#include "tabwidget.h"
#include "webpage.h"
#include "webview.h"
#include <QLabel>
#include <QLineEdit>
#include <QMenu>
#include <QTabBar>
#include <QWebEngineProfile>

TabWidget::TabWidget(QWidget *parent)
    : QTabWidget(parent)
{
    setTabsClosable(true);
    setMovable(true);
    setDocumentMode(true);
    setElideMode(Qt::ElideRight);
    
    // Set tab position to North (top)
    setTabPosition(QTabWidget::North);
    
    connect(this, &QTabWidget::tabCloseRequested, this, &TabWidget::closeTab);
    connect(this, &QTabWidget::currentChanged, this, &TabWidget::handleCurrentChanged);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &QWidget::customContextMenuRequested, this, &TabWidget::handleContextMenuRequested);
}

WebView *TabWidget::currentWebView() const
{
    return qobject_cast<WebView*>(currentWidget());
}

void TabWidget::handleCurrentChanged(int index)
{
    WebView *webView = qobject_cast<WebView*>(widget(index));
    if (!webView)
        return;

    emit urlChanged(webView->url());
    emit titleChanged(webView->title());
}

WebView *TabWidget::createTab()
{
    WebView *webView = new WebView(this);
    setupView(webView);
    addTab(webView, tr("(Untitled)"));
    setCurrentWidget(webView);
    return webView;
}

void TabWidget::setupView(WebView *webView)
{
    connect(webView, &QWebEngineView::titleChanged, [this, webView](const QString &title) {
        int index = indexOf(webView);
        if (index != -1) {
            setTabText(index, title.isEmpty() ? tr("(Untitled)") : title);
            setTabToolTip(index, title);
        }
        if (currentIndex() == index)
            emit titleChanged(title);
    });
    
    connect(webView, &QWebEngineView::urlChanged, [this, webView](const QUrl &url) {
        int index = indexOf(webView);
        if (index != -1 && currentIndex() == index)
            emit urlChanged(url);
    });
    
    connect(webView, &QWebEngineView::loadProgress, [this, webView](int progress) {
        int index = indexOf(webView);
        if (index != -1) {
            QString label = webView->title();
            if (progress < 100)
                label += QString(" (%1%)").arg(progress);
            setTabText(index, label.isEmpty() ? tr("(Untitled)") : label);
        }
        if (currentIndex() == index)
            emit loadProgress(progress);
    });
    
    connect(webView, &QWebEngineView::loadStarted, [this, webView]() {
        int index = indexOf(webView);
        if (currentIndex() == index)
            emit loadStarted();
    });
    
    connect(webView, &QWebEngineView::loadFinished, [this, webView]() {
        int index = indexOf(webView);
        if (currentIndex() == index)
            emit loadFinished();
    });
}

void TabWidget::closeTab(int index)
{
    if (WebView *view = qobject_cast<WebView*>(widget(index))) {
        bool hasFocus = view->hasFocus();
        removeTab(index);
        if (hasFocus && count() > 0)
            currentWebView()->setFocus();
        view->deleteLater();
    }
    
    if (count() == 0)
        createTab();
}

void TabWidget::handleContextMenuRequested(const QPoint &pos)
{
    QTabBar *tabBar = this->tabBar();
    int index = tabBar->tabAt(pos);
    if (index == -1)
        return;

    QMenu menu;
    menu.addAction(tr("&Reload Tab"), this, [this, index]() {
        reloadTab(index);
    });
    menu.addAction(tr("&Clone Tab"), this, [this, index]() {
        cloneTab(index);
    });
    menu.addSeparator();
    menu.addAction(tr("Close &Other Tabs"), this, [this, index]() {
        closeOtherTabs(index);
    });
    menu.addAction(tr("&Close Tab"), this, [this, index]() {
        closeTab(index);
    });
    menu.addSeparator();
    menu.addAction(tr("Reload All Tabs"), this, &TabWidget::reloadAllTabs);
    menu.exec(tabBar->mapToGlobal(pos));
}

void TabWidget::cloneTab(int index)
{
    if (WebView *view = qobject_cast<WebView*>(widget(index))) {
        WebView *tab = createTab();
        tab->setUrl(view->url());
    }
}

void TabWidget::closeOtherTabs(int index)
{
    for (int i = count() - 1; i >= 0; --i) {
        if (i != index)
            closeTab(i);
    }
}

void TabWidget::reloadAllTabs()
{
    for (int i = 0; i < count(); ++i)
        reloadTab(i);
}

void TabWidget::reloadTab(int index)
{
    if (WebView *view = qobject_cast<WebView*>(widget(index)))
        view->reload();
}
