#ifndef BROWSERWINDOW_H
#define BROWSERWINDOW_H

#include <QMainWindow>
#include <QUrl>

class QLineEdit;
class QPushButton;
class QProgressBar;
class TabWidget;

class BrowserWindow : public QMainWindow
{
    Q_OBJECT

public:
    BrowserWindow(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~BrowserWindow();
    
    TabWidget *tabWidget() const { return m_tabWidget; }
    
    QSize sizeHint() const override;

private slots:
    void handleUrlChanged(const QUrl &url);
    void handleTitleChanged(const QString &title);
    void handleNavigate();
    void handleBackward();
    void handleForward();
    void handleReload();
    void handleStop();
    void handleNewTab();
    void handleNewWindow();

private:
    void createActions();
    void createToolBar();
    void createMenuBar();
    
private:
    TabWidget *m_tabWidget;
    QLineEdit *m_urlLineEdit;
    QProgressBar *m_progressBar;
    
    QPushButton *m_backButton;
    QPushButton *m_forwardButton;
    QPushButton *m_reloadButton;
    QPushButton *m_stopButton;
};

#endif // BROWSERWINDOW_H
