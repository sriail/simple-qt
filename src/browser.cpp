#include "browser.h"
#include "browserwindow.h"

Browser::Browser()
{
}

BrowserWindow *Browser::createWindow(bool offTheRecord)
{
    Q_UNUSED(offTheRecord);
    auto *mainWin = new BrowserWindow();
    m_windows.append(mainWin);
    mainWin->show();
    return mainWin;
}
