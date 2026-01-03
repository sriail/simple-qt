#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include <QUrl>

class QLineEdit;
class WebView;

class TabWidget : public QTabWidget
{
    Q_OBJECT

public:
    TabWidget(QWidget *parent = nullptr);
    
    WebView *currentWebView() const;
    WebView *createTab();
    void closeTab(int index);

signals:
    void urlChanged(const QUrl &url);
    void titleChanged(const QString &title);

private slots:
    void handleCurrentChanged(int index);
    void handleContextMenuRequested(const QPoint &pos);
    void cloneTab(int index);
    void closeOtherTabs(int index);
    void reloadAllTabs();
    void reloadTab(int index);

private:
    void setupView(WebView *webView);
    
    QLineEdit *m_urlLineEdit = nullptr;
};

#endif // TABWIDGET_H
