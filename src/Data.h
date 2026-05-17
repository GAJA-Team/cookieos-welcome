#pragma once
#include <QString>
#include <QList>

struct ChangelogItem { QString type, text, ver; };
struct AppItem       { QString icon, name, desc, color; };

inline QList<ChangelogItem> changelogData()
{
    return {
        {"add",   "Add in OOBE mate & cinnamon",             "1.0.1 · OOBE"},
        {"fix",   "Fix the cookieos-welcome", "1.0.1 · welcome"},
    };
}

inline QList<AppItem> appsData()
{
    return {
        {"🍪", "CookieOS Package Manager",   "Package Manager",    "#1a2e1a"},
        {"", "CookieOS Customizer",    "System Customizer", "#1a1a2e"},
        {"⚙️", "CookieOS Updater",  "System Updater", "#2e2a1a"},
    };
}
