#include "browser.h"
#include "browserwindow.h"

Browser::Browser()
{
}

Browser& Browser::instance()
{
    static Browser browser;
    return browser;
}

BrowserWindow *Browser::createWindow(bool offTheRecord)
{
    Q_UNUSED(offTheRecord);
    auto *mainWin = new BrowserWindow();
    m_windows.append(mainWin);
    mainWin->show();
    return mainWin;
}
