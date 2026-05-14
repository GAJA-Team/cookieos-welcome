#pragma once
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QMap>

struct OsRelease {
    QString prettyName      = "CookieOS";
    QString versionId       = "1.0";
    QString versionCodename = "";
    QString idLike          = "debian";
    QString homeUrl         = "";
};

inline OsRelease parseOsRelease()
{
    OsRelease r;
    QFile f("/etc/os-release");
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return r;

    QTextStream in(&f);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#')) continue;
        int eq = line.indexOf('=');
        if (eq < 0) continue;
        QString k = line.left(eq).trimmed();
        QString v = line.mid(eq + 1).trimmed();
        if ((v.startsWith('"') && v.endsWith('"')) ||
            (v.startsWith('\'') && v.endsWith('\'')))
            v = v.mid(1, v.size() - 2);

        if      (k == "PRETTY_NAME")      r.prettyName      = v;
        else if (k == "VERSION_ID")       r.versionId       = v;
        else if (k == "VERSION_CODENAME") r.versionCodename = v;
        else if (k == "ID_LIKE")          r.idLike          = v;
        else if (k == "HOME_URL")         r.homeUrl         = v;
    }
    return r;
}
