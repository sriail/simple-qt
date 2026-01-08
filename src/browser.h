#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>
#include <QTabBar>
#include <QStackedWidget>
#include <QLineEdit>
#include <QToolBar>
#include <QAction>
#include <QSettings>
#include "webview.h"

class AdBlocker;

class Browser : public QMainWindow
{
    Q_OBJECT

public:
    explicit Browser(QWidget *parent = nullptr);
    ~Browser();

private slots:
    void createNewTab();
    void createNewTabWithUrl(const QUrl &url);
    void closeTab(int index);
    void updateUrlBar();
    void navigateToUrl();
    void goBack();
    void goForward();
    void reload();
    void goHome();
    void updateNavigationActions();
    void tabChanged(int index);
    void toggleAdBlocker();
    void clearAllCookies();

private:
    void setupUi();
    WebView *currentWebView();
    bool isDarkMode() const;
    void loadSettings();
    void saveSettings();
    QString getSearchEngineUrl() const;

    QTabBar *tabBar;
    QStackedWidget *stackedWidget;
    QLineEdit *urlBar;
    QToolBar *navigationBar;
    
    QAction *backAction;
    QAction *forwardAction;
    QAction *reloadAction;
    QAction *homeAction;
    QAction *shieldAction;
    QAction *moreAction;
    
    QString homePage;
    bool darkMode;
    bool adBlockerEnabled;
    
    AdBlocker *adBlocker;
    QSettings *settings;
};

#endif // BROWSER_H
