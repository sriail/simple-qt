#ifndef ADBLOCKER_H
#define ADBLOCKER_H

#include <QWebEngineUrlRequestInterceptor>
#include <QSet>
#include <QString>

class AdBlocker : public QWebEngineUrlRequestInterceptor
{
    Q_OBJECT

public:
    explicit AdBlocker(QObject *parent = nullptr);
    
    void interceptRequest(QWebEngineUrlRequestInfo &info) override;
    
    void setEnabled(bool enabled);
    bool isEnabled() const;

private:
    bool m_enabled;
    QSet<QString> m_blockedDomains;
    
    void initBlockList();
    bool shouldBlock(const QUrl &url) const;
};

#endif // ADBLOCKER_H
