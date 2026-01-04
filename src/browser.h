#ifndef BROWSER_H
#define BROWSER_H

#include <QMainWindow>
#include <QTabBar>
#include <QStackedWidget>
#include <QLineEdit>
#include <QToolBar>
#include <QAction>
#include "webview.h"

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

private:
    void setupUi();
    WebView *currentWebView();

    QTabBar *tabBar;
    QStackedWidget *stackedWidget;
    QLineEdit *urlBar;
    QToolBar *navigationBar;
    
    QAction *backAction;
    QAction *forwardAction;
    QAction *reloadAction;
    QAction *homeAction;
    
    QString homePage;
};

#endif // BROWSER_H
