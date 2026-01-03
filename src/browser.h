#ifndef BROWSER_H
#define BROWSER_H

#include <QVector>
#include <QUrl>

class BrowserWindow;

class Browser
{
public:
    Browser();
    
    QVector<BrowserWindow*> windows() { return m_windows; }
    
    BrowserWindow *createWindow(bool offTheRecord = false);

private:
    QVector<BrowserWindow*> m_windows;
};

#endif // BROWSER_H
