#include "browser.h"
#include "adblocker.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QToolButton>
#include <QStyle>
#include <QUrl>
#include <QWebEngineProfile>
#include <QWebEngineHistory>
#include <QWebEngineCookieStore>
#include <QWebEnginePage>
#include <QFile>
#include <QIcon>
#include <QPalette>
#include <QApplication>
#include <QSettings>

Browser::Browser(QWidget *parent)
    : QMainWindow(parent)
    , adBlockerEnabled(false)
    , searchEngine("duckduckgo")
    , themePreference("system")
{
    homePage = "qrc:/html/home.html";
    
    // Initialize settings
    settings = new QSettings("Simple Qt", "Simple Browser", this);
    
    // Load settings
    loadSettings();
    
    // Detect dark mode once during initialization
    darkMode = isDarkMode();
    
    // Initialize ad blocker
    adBlocker = new AdBlocker(this);
    adBlocker->setEnabled(adBlockerEnabled);
    QWebEngineProfile::defaultProfile()->setUrlRequestInterceptor(adBlocker);
    
    // Load stylesheet with appropriate icons based on dark mode
    QString iconPrefix = darkMode ? "white-" : "";
    QString styleSheet = QString(
        "QTabBar::close-button {"
        "    image: url(:/icons/%1x.png);"
        "    subcontrol-position: right;"
        "    width: 12px;"
        "    height: 12px;"
        "}"
        "QTabBar::close-button:hover {"
        "    image: url(:/icons/%1x.png);"
        "}"
    ).arg(iconPrefix);
    
    setStyleSheet(styleSheet);
    
    setupUi();
    createNewTab();
}

Browser::~Browser()
{
    saveSettings();
}

void Browser::setupUi()
{
    setWindowTitle("Simple Browser");
    resize(1200, 800);

    // Determine icon prefix based on dark mode (cached during construction)
    QString iconPrefix = darkMode ? "white-" : "";

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
    newTabButton->setIcon(QIcon(QString(":/icons/%1plus.png").arg(iconPrefix)));
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

    // Navigation actions with Tabler icons
    backAction = navigationBar->addAction(QIcon(QString(":/icons/%1arrow-left.png").arg(iconPrefix)), "Back");
    connect(backAction, &QAction::triggered, this, &Browser::goBack);

    forwardAction = navigationBar->addAction(QIcon(QString(":/icons/%1arrow-right.png").arg(iconPrefix)), "Forward");
    connect(forwardAction, &QAction::triggered, this, &Browser::goForward);

    reloadAction = navigationBar->addAction(QIcon(QString(":/icons/%1reload.png").arg(iconPrefix)), "Reload");
    connect(reloadAction, &QAction::triggered, this, &Browser::reload);

    navigationBar->addSeparator();

    // URL bar
    urlBar = new QLineEdit(this);
    urlBar->setPlaceholderText("Enter URL or search...");
    connect(urlBar, &QLineEdit::returnPressed, this, &Browser::navigateToUrl);
    navigationBar->addWidget(urlBar);

    // Home action after URL bar
    homeAction = navigationBar->addAction(QIcon(QString(":/icons/%1home.png").arg(iconPrefix)), "Home");
    connect(homeAction, &QAction::triggered, this, &Browser::goHome);

    // Shield action (for ad blocking toggle)
    QString shieldIcon = adBlockerEnabled ? "shield-check" : "shield";
    shieldAction = navigationBar->addAction(QIcon(QString(":/icons/%1%2.png").arg(iconPrefix, shieldIcon)), 
                                             adBlockerEnabled ? "Ad Blocking: ON" : "Ad Blocking: OFF");
    shieldAction->setCheckable(true);
    shieldAction->setChecked(adBlockerEnabled);
    connect(shieldAction, &QAction::triggered, this, &Browser::toggleAdBlocker);

    // More action (for future dropdown menu)
    moreAction = navigationBar->addAction(QIcon(QString(":/icons/%1dots-vertical.png").arg(iconPrefix)), "More");
    moreAction->setEnabled(false);  // Disabled for now, will be implemented later

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
        
        // Handle clear-cookies URL scheme
        if (url.scheme() == "simple-browser" && url.host() == "clear-cookies") {
            clearAllCookies();
            // Navigate back to settings
            webView->setUrl(QUrl("qrc:/html/settings.html"));
        }
    });

    connect(webView, &WebView::loadFinished, this, [this, webView](bool ok) {
        updateNavigationActions();
        if (ok) {
            injectThemeScript(webView);
        }
    });

    connect(webView, &WebView::createNewTab, this, &Browser::createNewTabWithUrl);

    // Add to stacked widget first to get the index
    int index = stackedWidget->addWidget(webView);
    
    // Add tab at the same index position to keep them synchronized
    tabBar->insertTab(index, "New Tab");
    
    // Set both to the new index
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
        // Remove from both tab bar and stacked widget at the same index
        QWidget *widget = stackedWidget->widget(index);
        tabBar->removeTab(index);
        stackedWidget->removeWidget(widget);
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
        // Search with selected search engine
        url = QUrl(getSearchEngineUrl() + QString(QUrl::toPercentEncoding(text)));
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

bool Browser::isDarkMode() const
{
    // Check if the system is using a dark color scheme
    QPalette palette = QApplication::palette();
    QColor windowColor = palette.color(QPalette::Window);
    
    // Calculate luminance: if the background is darker, we're in dark mode
    // Using relative luminance formula: 0.299*R + 0.587*G + 0.114*B
    qreal luminance = (0.299 * windowColor.redF() + 
                       0.587 * windowColor.greenF() + 
                       0.114 * windowColor.blueF());
    
    // If luminance is less than 0.5, it's a dark theme
    return luminance < 0.5;
}

void Browser::loadSettings()
{
    adBlockerEnabled = settings->value("adBlocker/enabled", false).toBool();
    searchEngine = settings->value("search/engine", "duckduckgo").toString();
    themePreference = settings->value("appearance/theme", "system").toString();
}

void Browser::saveSettings()
{
    settings->setValue("adBlocker/enabled", adBlockerEnabled);
    settings->setValue("search/engine", searchEngine);
    settings->setValue("appearance/theme", themePreference);
    settings->sync();
}

QString Browser::getSearchEngineUrl() const
{
    if (searchEngine == "google") {
        return "https://www.google.com/search?q=";
    } else if (searchEngine == "bing") {
        return "https://www.bing.com/search?q=";
    } else if (searchEngine == "brave") {
        return "https://search.brave.com/search?q=";
    } else if (searchEngine == "startpage") {
        return "https://www.startpage.com/do/search?q=";
    }
    // Default to DuckDuckGo
    return "https://duckduckgo.com/?q=";
}

void Browser::toggleAdBlocker()
{
    adBlockerEnabled = !adBlockerEnabled;
    adBlocker->setEnabled(adBlockerEnabled);
    
    // Update icon based on state
    QString iconPrefix = darkMode ? "white-" : "";
    QString shieldIcon = adBlockerEnabled ? "shield-check" : "shield";
    shieldAction->setIcon(QIcon(QString(":/icons/%1%2.png").arg(iconPrefix, shieldIcon)));
    shieldAction->setText(adBlockerEnabled ? "Ad Blocking: ON" : "Ad Blocking: OFF");
    shieldAction->setChecked(adBlockerEnabled);
    
    // Save setting
    saveSettings();
    
    // Reload current page to apply ad blocking
    WebView *webView = currentWebView();
    if (webView) {
        webView->reload();
    }
}

void Browser::clearAllCookies()
{
    QWebEngineProfile *profile = QWebEngineProfile::defaultProfile();
    if (profile) {
        profile->cookieStore()->deleteAllCookies();
    }
}

void Browser::injectThemeScript(WebView *webView)
{
    if (!webView) return;
    
    // Inject theme CSS based on preference
    QString script;
    
    bool shouldUseDark = false;
    if (themePreference == "dark") {
        shouldUseDark = true;
    } else if (themePreference == "system") {
        shouldUseDark = darkMode;
    }
    
    if (shouldUseDark) {
        script = R"(
            (function() {
                // Check if we're on a Simple Browser internal page
                if (window.location.protocol === 'qrc:') {
                    if (!document.body.classList.contains('dark')) {
                        document.body.classList.add('dark');
                    }
                }
            })();
        )";
    } else {
        script = R"(
            (function() {
                // Check if we're on a Simple Browser internal page
                if (window.location.protocol === 'qrc:') {
                    document.body.classList.remove('dark');
                }
            })();
        )";
    }
    
    webView->page()->runJavaScript(script);
}
