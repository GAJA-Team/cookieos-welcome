#pragma once
#include <QString>
#include <QList>

struct ChangelogItem { QString type, text, ver; };
struct AppItem       { QString icon, name, desc, color; };

inline QList<ChangelogItem> changelogData()
{
    return {
        {"add",   "Dodano wiele aplikacji",             "1.4.2 · kernel"},
        {"fix",   "Naprawiono błąd zawieszania przy starcie SDDM", "1.4.1 · display"},
        {"perf",  "Zoptymalizowano czas uruchamiania o 31%",        "1.4.1 · boot"},
        {"add",   "Wbudowany kreator instalacji cookie-setup",      "1.4.0 · installer"},
        {"break", "Zmieniono ścieżkę konfiguracji na /etc/cookie/","1.4.0 · breaking"},
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
