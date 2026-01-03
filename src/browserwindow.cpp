#include "browserwindow.h"
#include "browser.h"
#include "tabwidget.h"
#include "webview.h"
#include <QAction>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QProgressBar>
#include <QPushButton>
#include <QStatusBar>
#include <QToolBar>
#include <QVBoxLayout>

BrowserWindow::BrowserWindow(QWidget *parent, Qt::WindowFlags flags)
    : QMainWindow(parent, flags)
{
    setWindowTitle(tr("Simple Browser"));
    resize(1024, 768);
    
    // Create central widget with vertical layout
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    
    // Create tab widget (this contains the tab bar at the top)
    m_tabWidget = new TabWidget(this);
    
    // Create URL bar widget
    QWidget *urlBarWidget = new QWidget(this);
    QHBoxLayout *urlBarLayout = new QHBoxLayout(urlBarWidget);
    urlBarLayout->setContentsMargins(5, 5, 5, 5);
    
    // Navigation buttons
    m_backButton = new QPushButton(tr("←"), this);
    m_backButton->setToolTip(tr("Back"));
    m_backButton->setMaximumWidth(40);
    connect(m_backButton, &QPushButton::clicked, this, &BrowserWindow::handleBackward);
    
    m_forwardButton = new QPushButton(tr("→"), this);
    m_forwardButton->setToolTip(tr("Forward"));
    m_forwardButton->setMaximumWidth(40);
    connect(m_forwardButton, &QPushButton::clicked, this, &BrowserWindow::handleForward);
    
    m_reloadButton = new QPushButton(tr("⟳"), this);
    m_reloadButton->setToolTip(tr("Reload"));
    m_reloadButton->setMaximumWidth(40);
    connect(m_reloadButton, &QPushButton::clicked, this, &BrowserWindow::handleReload);
    
    m_stopButton = new QPushButton(tr("✕"), this);
    m_stopButton->setToolTip(tr("Stop"));
    m_stopButton->setMaximumWidth(40);
    connect(m_stopButton, &QPushButton::clicked, this, &BrowserWindow::handleStop);
    
    // URL line edit
    m_urlLineEdit = new QLineEdit(this);
    m_urlLineEdit->setPlaceholderText(tr("Enter URL"));
    connect(m_urlLineEdit, &QLineEdit::returnPressed, this, &BrowserWindow::handleNavigate);
    
    // New tab button
    QPushButton *newTabButton = new QPushButton(tr("+"), this);
    newTabButton->setToolTip(tr("New Tab"));
    newTabButton->setMaximumWidth(40);
    connect(newTabButton, &QPushButton::clicked, this, &BrowserWindow::handleNewTab);
    
    // Add widgets to URL bar layout
    urlBarLayout->addWidget(m_backButton);
    urlBarLayout->addWidget(m_forwardButton);
    urlBarLayout->addWidget(m_reloadButton);
    urlBarLayout->addWidget(m_stopButton);
    urlBarLayout->addWidget(m_urlLineEdit);
    urlBarLayout->addWidget(newTabButton);
    
    // Add widgets to main layout: Tab bar first (at top), then URL bar below it
    layout->addWidget(m_tabWidget);
    layout->addWidget(urlBarWidget);
    
    setCentralWidget(centralWidget);
    
    // Progress bar in status bar
    m_progressBar = new QProgressBar(this);
    m_progressBar->setMaximumHeight(15);
    m_progressBar->setMaximumWidth(150);
    m_progressBar->setTextVisible(false);
    statusBar()->addPermanentWidget(m_progressBar);
    
    // Connect tab widget signals
    connect(m_tabWidget, &TabWidget::urlChanged, this, &BrowserWindow::handleUrlChanged);
    connect(m_tabWidget, &TabWidget::titleChanged, this, &BrowserWindow::handleTitleChanged);
    
    createMenuBar();
    
    // Create initial tab and load homepage
    WebView *view = m_tabWidget->createTab();
    view->load(QUrl("qrc:/homepage.html"));
}

BrowserWindow::~BrowserWindow()
{
}

QSize BrowserWindow::sizeHint() const
{
    return QSize(1024, 768);
}

void BrowserWindow::handleUrlChanged(const QUrl &url)
{
    m_urlLineEdit->setText(url.toString());
    
    if (WebView *view = m_tabWidget->currentWebView()) {
        m_backButton->setEnabled(view->history()->canGoBack());
        m_forwardButton->setEnabled(view->history()->canGoForward());
        
        connect(view, &QWebEngineView::loadStarted, [this]() {
            m_progressBar->setVisible(true);
            m_progressBar->setValue(0);
        });
        
        connect(view, &QWebEngineView::loadProgress, [this](int progress) {
            m_progressBar->setValue(progress);
        });
        
        connect(view, &QWebEngineView::loadFinished, [this]() {
            m_progressBar->setVisible(false);
        });
    }
}

void BrowserWindow::handleTitleChanged(const QString &title)
{
    if (title.isEmpty())
        setWindowTitle(tr("Simple Browser"));
    else
        setWindowTitle(title + tr(" - Simple Browser"));
}

void BrowserWindow::handleNavigate()
{
    QString urlText = m_urlLineEdit->text();
    if (urlText.isEmpty())
        return;
    
    QUrl url;
    if (urlText.startsWith("http://") || urlText.startsWith("https://") || urlText.startsWith("file://"))
        url = QUrl::fromUserInput(urlText);
    else
        url = QUrl::fromUserInput("https://" + urlText);
    
    if (WebView *view = m_tabWidget->currentWebView())
        view->load(url);
}

void BrowserWindow::handleBackward()
{
    if (WebView *view = m_tabWidget->currentWebView())
        view->back();
}

void BrowserWindow::handleForward()
{
    if (WebView *view = m_tabWidget->currentWebView())
        view->forward();
}

void BrowserWindow::handleReload()
{
    if (WebView *view = m_tabWidget->currentWebView())
        view->reload();
}

void BrowserWindow::handleStop()
{
    if (WebView *view = m_tabWidget->currentWebView())
        view->stop();
}

void BrowserWindow::handleNewTab()
{
    WebView *view = m_tabWidget->createTab();
    view->load(QUrl("qrc:/homepage.html"));
}

void BrowserWindow::handleNewWindow()
{
    // Create new browser window via Browser singleton
    // This would need a reference to Browser instance
}

void BrowserWindow::createActions()
{
    // Actions are handled via direct connections in constructor
}

void BrowserWindow::createToolBar()
{
    // Toolbar is integrated into the layout
}

void BrowserWindow::createMenuBar()
{
    QMenuBar *menuBar = this->menuBar();
    
    // File menu
    QMenu *fileMenu = menuBar->addMenu(tr("&File"));
    fileMenu->addAction(tr("&New Tab"), this, &BrowserWindow::handleNewTab, QKeySequence::AddTab);
    fileMenu->addAction(tr("New &Window"), this, &BrowserWindow::handleNewWindow, QKeySequence::New);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Quit"), qApp, &QApplication::quit, QKeySequence::Quit);
    
    // View menu
    QMenu *viewMenu = menuBar->addMenu(tr("&View"));
    viewMenu->addAction(tr("&Reload"), this, &BrowserWindow::handleReload, QKeySequence::Refresh);
    viewMenu->addAction(tr("&Stop"), this, &BrowserWindow::handleStop, QKeySequence(Qt::Key_Escape));
    
    // Help menu
    QMenu *helpMenu = menuBar->addMenu(tr("&Help"));
    helpMenu->addAction(tr("About Simple Browser"), [this]() {
        QMessageBox::about(this, tr("About Simple Browser"),
            tr("A simple web browser built with Qt WebEngine"));
    });
    helpMenu->addAction(tr("About Qt"), qApp, &QApplication::aboutQt);
}
