#ifndef BROWSER_H
#define BROWSER_H

#include <QVector>
#include <QUrl>

class BrowserWindow;

class Browser
{
public:
    static Browser& instance();
    
    QVector<BrowserWindow*> windows() { return m_windows; }
    
    BrowserWindow *createWindow(bool offTheRecord = false);

private:
    Browser();
    Browser(const Browser&) = delete;
    Browser& operator=(const Browser&) = delete;
    
    QVector<BrowserWindow*> m_windows;
};

#endif // BROWSER_H
