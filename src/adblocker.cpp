#include "adblocker.h"
#include <QUrl>

AdBlocker::AdBlocker(QObject *parent)
    : QWebEngineUrlRequestInterceptor(parent)
    , m_enabled(false)
{
    initBlockList();
}

void AdBlocker::initBlockList()
{
    // Common ad and tracker domains
    m_blockedDomains = {
        // Google Ads
        "googlesyndication.com",
        "googleadservices.com",
        "doubleclick.net",
        "googleads.g.doubleclick.net",
        "adservice.google.com",
        "pagead2.googlesyndication.com",
        
        // Facebook tracking
        "connect.facebook.net",
        "pixel.facebook.com",
        "facebook.com/tr",
        
        // Analytics & Tracking
        "google-analytics.com",
        "googletagmanager.com",
        "googletagservices.com",
        "analytics.twitter.com",
        "analytics.tiktok.com",
        "segment.io",
        "segment.com",
        "mixpanel.com",
        "hotjar.com",
        "fullstory.com",
        "mouseflow.com",
        "crazyegg.com",
        "optimizely.com",
        "quantserve.com",
        "scorecardresearch.com",
        "chartbeat.com",
        "newrelic.com",
        "nr-data.net",
        
        // Ad networks
        "adnxs.com",
        "adsrvr.org",
        "advertising.com",
        "amazon-adsystem.com",
        "bidswitch.net",
        "bluekai.com",
        "casalemedia.com",
        "contextweb.com",
        "criteo.com",
        "criteo.net",
        "demdex.net",
        "exelator.com",
        "eyeota.net",
        "krxd.net",
        "liadm.com",
        "liveramp.com",
        "mathtag.com",
        "media.net",
        "moatads.com",
        "openx.net",
        "outbrain.com",
        "pubmatic.com",
        "rubiconproject.com",
        "sharethrough.com",
        "simpli.fi",
        "smartadserver.com",
        "spotxchange.com",
        "taboola.com",
        "tapad.com",
        "thetradedesk.com",
        "tidaltv.com",
        "tribalfusion.com",
        "turn.com",
        "undertone.com",
        "yieldmo.com",
        
        // Other trackers
        "adform.net",
        "adsafeprotected.com",
        "adzerk.net",
        "agkn.com",
        "bing.com/bat.js",
        "bounceexchange.com",
        "branch.io",
        "cdn.branch.io",
        "cloudflareinsights.com",
        "convertkit.com",
        "drift.com",
        "driftt.com",
        "hubspot.com",
        "hs-analytics.net",
        "hs-scripts.com",
        "intercom.io",
        "intercomcdn.com",
        "linkedin.com/px",
        "onesignal.com",
        "pardot.com",
        "ping.chartbeat.net",
        "pixel.quantserve.com",
        "sentry.io",
        "sentry-cdn.com",
        "snapchat.com/scp",
        "tealiumiq.com",
        "zdassets.com"
    };
}

bool AdBlocker::shouldBlock(const QUrl &url) const
{
    QString host = url.host().toLower();
    QString path = url.path().toLower();
    QString fullUrl = host + path;
    
    // Check against blocked domains
    for (const QString &domain : m_blockedDomains) {
        if (host.endsWith(domain) || host == domain || fullUrl.contains(domain)) {
            return true;
        }
    }
    
    // Block common ad-related paths
    if (path.contains("/ads/") || 
        path.contains("/ad/") ||
        path.contains("/advertisement") ||
        path.contains("/tracking") ||
        path.contains("/tracker") ||
        path.contains("/pixel") ||
        path.contains("/beacon") ||
        path.contains("/analytics") ||
        path.endsWith("/ad.js") ||
        path.endsWith("/ads.js") ||
        path.endsWith("/tracking.js") ||
        path.endsWith("/analytics.js")) {
        return true;
    }
    
    return false;
}

void AdBlocker::interceptRequest(QWebEngineUrlRequestInfo &info)
{
    if (!m_enabled) {
        return;
    }
    
    QUrl url = info.requestUrl();
    
    if (shouldBlock(url)) {
        info.block(true);
    }
}

void AdBlocker::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

bool AdBlocker::isEnabled() const
{
    return m_enabled;
}
