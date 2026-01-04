#include "browser.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QStyle>
#include <QUrl>
#include <QWebEngineProfile>
#include <QWebEngineHistory>
#include <QTabBar>
#include <QStackedWidget>

Browser::Browser(QWidget *parent)
    : QMainWindow(parent)
{
    homePage = "qrc:/html/home.html";
    setupUi();
    createNewTab();
}

Browser::~Browser()
{
}

void Browser::setupUi()
{
    setWindowTitle("Simple Browser");
    resize(1200, 800);

    // Create central widget and main layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Create tab bar at the top
    QWidget *tabBarWidget = new QWidget(this);
    QHBoxLayout *tabBarLayout = new QHBoxLayout(tabBarWidget);
    tabBarLayout->setContentsMargins(0, 0, 0, 0);
    tabBarLayout->setSpacing(0);
    
    tabBar = new QTabBar(this);
    tabBar->setTabsClosable(true);
    tabBar->setMovable(true);
    tabBar->setDocumentMode(true);
    tabBar->setExpanding(false);
    tabBarLayout->addWidget(tabBar);
    
    // Add new tab button
    QToolButton *newTabButton = new QToolButton(this);
    newTabButton->setText("+");
    newTabButton->setToolTip("New Tab");
    tabBarLayout->addWidget(newTabButton);
    tabBarLayout->addStretch();
    
    connect(newTabButton, &QToolButton::clicked, this, &Browser::createNewTab);
    connect(tabBar, &QTabBar::tabCloseRequested, this, &Browser::closeTab);
    connect(tabBar, &QTabBar::currentChanged, this, &Browser::tabChanged);

    // Create navigation toolbar (below tab bar)
    navigationBar = new QToolBar(this);
    navigationBar->setMovable(false);
    navigationBar->setIconSize(QSize(16, 16));

    // Navigation actions
    backAction = navigationBar->addAction(style()->standardIcon(QStyle::SP_ArrowBack), "Back");
    connect(backAction, &QAction::triggered, this, &Browser::goBack);

    forwardAction = navigationBar->addAction(style()->standardIcon(QStyle::SP_ArrowForward), "Forward");
    connect(forwardAction, &QAction::triggered, this, &Browser::goForward);

    reloadAction = navigationBar->addAction(style()->standardIcon(QStyle::SP_BrowserReload), "Reload");
    connect(reloadAction, &QAction::triggered, this, &Browser::reload);

    homeAction = navigationBar->addAction(style()->standardIcon(QStyle::SP_DirHomeIcon), "Home");
    connect(homeAction, &QAction::triggered, this, &Browser::goHome);

    navigationBar->addSeparator();

    // URL bar
    urlBar = new QLineEdit(this);
    urlBar->setPlaceholderText("Enter URL or search...");
    connect(urlBar, &QLineEdit::returnPressed, this, &Browser::navigateToUrl);
    navigationBar->addWidget(urlBar);

    // Create stacked widget for web content (below navigation bar)
    stackedWidget = new QStackedWidget(this);

    // Add widgets to main layout in order: tab bar, navigation bar, web content
    mainLayout->addWidget(tabBarWidget);
    mainLayout->addWidget(navigationBar);
    mainLayout->addWidget(stackedWidget);

    setCentralWidget(centralWidget);
}

void Browser::createNewTab()
{
    createNewTabWithUrl(QUrl(homePage));
}

void Browser::createNewTabWithUrl(const QUrl &url)
{
    WebView *webView = new WebView();
    
    // Connect signals
    connect(webView, &WebView::titleChanged, this, [this, webView](const QString &title) {
        int index = stackedWidget->indexOf(webView);
        if (index != -1) {
            tabBar->setTabText(index, title.isEmpty() ? "New Tab" : title);
        }
    });

    connect(webView, &WebView::urlChanged, this, [this, webView](const QUrl &url) {
        if (webView == currentWebView()) {
            updateUrlBar();
            updateNavigationActions();
        }
    });

    connect(webView, &WebView::loadFinished, this, [this]() {
        updateNavigationActions();
    });

    connect(webView, &WebView::createNewTab, this, &Browser::createNewTabWithUrl);

    // Add tab and web view
    int index = stackedWidget->addWidget(webView);
    tabBar->addTab("New Tab");
    tabBar->setCurrentIndex(index);
    stackedWidget->setCurrentIndex(index);

    // Load URL
    if (url.isEmpty()) {
        webView->setUrl(QUrl(homePage));
    } else {
        webView->setUrl(url);
    }
}

void Browser::closeTab(int index)
{
    if (tabBar->count() > 1) {
        QWidget *widget = stackedWidget->widget(index);
        stackedWidget->removeWidget(widget);
        tabBar->removeTab(index);
        widget->deleteLater();
    } else {
        // Close application if last tab is closed
        close();
    }
}

void Browser::updateUrlBar()
{
    WebView *webView = currentWebView();
    if (webView) {
        urlBar->setText(webView->url().toString());
        updateNavigationActions();
    }
}

void Browser::navigateToUrl()
{
    WebView *webView = currentWebView();
    if (!webView) return;

    QString text = urlBar->text().trimmed();
    if (text.isEmpty()) return;

    QUrl url;
    
    // Check if it's a URL or search term
    if (text.contains('.') && !text.contains(' ')) {
        // Likely a URL
        if (!text.startsWith("http://") && !text.startsWith("https://") && !text.startsWith("qrc://")) {
            text = "https://" + text;
        }
        url = QUrl(text);
    } else {
        // Search with DuckDuckGo
        url = QUrl("https://duckduckgo.com/?q=" + QString(QUrl::toPercentEncoding(text)));
    }

    webView->setUrl(url);
}

void Browser::goBack()
{
    WebView *webView = currentWebView();
    if (webView) {
        webView->back();
    }
}

void Browser::goForward()
{
    WebView *webView = currentWebView();
    if (webView) {
        webView->forward();
    }
}

void Browser::reload()
{
    WebView *webView = currentWebView();
    if (webView) {
        webView->reload();
    }
}

void Browser::goHome()
{
    WebView *webView = currentWebView();
    if (webView) {
        webView->setUrl(QUrl(homePage));
    }
}

void Browser::updateNavigationActions()
{
    WebView *webView = currentWebView();
    if (webView) {
        backAction->setEnabled(webView->history()->canGoBack());
        forwardAction->setEnabled(webView->history()->canGoForward());
    } else {
        backAction->setEnabled(false);
        forwardAction->setEnabled(false);
    }
}

void Browser::tabChanged(int index)
{
    stackedWidget->setCurrentIndex(index);
    updateUrlBar();
}

WebView *Browser::currentWebView()
{
    return qobject_cast<WebView*>(stackedWidget->currentWidget());
}
