#pragma once
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDir>
#include <QProcess>
#include <QRegularExpression>
#include <sys/statvfs.h>
#include <unistd.h>

struct SysInfo {
    QString kernel      = "unknown";
    double  ramUsedGb   = 0, ramTotalGb = 0, ramPct = 0;
    double  diskUsedGb  = 0, diskTotalGb = 0, diskPct = 0;
    double  cpuPct      = 0;
    QString uptime      = "n/a";
    QString packages    = "n/a";
    int     processes   = 0;
    int     updates     = 0;
};

namespace SysReader {

inline QString kernel()
{
    QFile f("/proc/version");
    if (!f.open(QIODevice::ReadOnly)) return "unknown";
    auto parts = QString(f.readLine()).split(' ');
    return parts.size() > 2 ? parts[2] : "unknown";
}

inline void ram(double &usedGb, double &totalGb, double &pct)
{
    QFile f("/proc/meminfo");
    if (!f.open(QIODevice::ReadOnly)) return;
    QTextStream in(&f);
    long totalKb = 0, availKb = 0;
    static QRegularExpression spc("\\s+");
    while (!in.atEnd()) {
        auto p = in.readLine().split(spc);
        if (p.size() < 2) continue;
        if (p[0] == "MemTotal:")     totalKb = p[1].toLong();
        if (p[0] == "MemAvailable:") availKb = p[1].toLong();
    }
    if (!totalKb) return;
    long usedKb = totalKb - availKb;
    totalGb = totalKb / 1048576.0;
    usedGb  = usedKb  / 1048576.0;
    pct     = usedKb * 100.0 / totalKb;
}

inline void disk(double &usedGb, double &totalGb, double &pct)
{
    struct statvfs st{};
    if (statvfs("/", &st)) return;
    auto total = (unsigned long long)st.f_blocks * st.f_frsize;
    auto avail = (unsigned long long)st.f_bavail * st.f_frsize;
    auto used  = total - avail;
    totalGb = total / 1073741824.0;
    usedGb  = used  / 1073741824.0;
    pct     = total ? used * 100.0 / total : 0;
}

inline double cpu()
{
    auto snap = []() -> QPair<long long,long long> {
        QFile f("/proc/stat");
        if (!f.open(QIODevice::ReadOnly)) return {0,0};
        static QRegularExpression spc("\\s+");
        auto p = QString(f.readLine()).split(spc);
        if (p.size() < 8) return {0,0};
        long long u=p[1].toLongLong(), n=p[2].toLongLong(),
                  s=p[3].toLongLong(), id=p[4].toLongLong(),
                  iow=p[5].toLongLong(), irq=p[6].toLongLong(),
                  sirq=p[7].toLongLong();
        long long idle = id + iow;
        long long tot  = u+n+s+id+iow+irq+sirq;
        return {idle, tot};
    };
    auto [i1,t1] = snap(); usleep(200000); auto [i2,t2] = snap();
    long long dt = t2-t1, di = i2-i1;
    return dt ? (dt-di)*100.0/dt : 0;
}

inline QString uptime()
{
    QFile f("/proc/uptime");
    if (!f.open(QIODevice::ReadOnly)) return "n/a";
    double s = QString(f.readLine()).split(' ')[0].toDouble();
    return QString("%1h %2m").arg((int)(s/3600)).arg((int)((int)s%3600/60));
}

inline int processes()
{
    int n = 0;
    for (auto &e : QDir("/proc").entryList(QDir::Dirs|QDir::NoDotAndDotDot))
        if (e[0].isDigit()) ++n;
    return n;
}

inline QString packages()
{
    QProcess p;
    p.start("bash", {"-c", "dpkg --get-selections 2>/dev/null | grep -c install"});
    p.waitForFinished(3000);
    QString out = p.readAllStandardOutput().trimmed();
    if (!out.isEmpty() && out != "0") return out;
    p.start("bash", {"-c", "pacman -Q 2>/dev/null | wc -l"});
    p.waitForFinished(3000);
    out = p.readAllStandardOutput().trimmed();
    return out.isEmpty() ? "n/a" : out;
}

inline int updates()
{
    QProcess p;
    p.start("bash", {"-c",
        "apt-get --simulate upgrade 2>/dev/null | grep -oP '\\d+(?= upgraded)'"});
    p.waitForFinished(8000);
    return p.readAllStandardOutput().trimmed().toInt();
}

} // namespace SysReader

inline SysInfo collectSysInfo()
{
    SysInfo s;
    s.kernel    = SysReader::kernel();
    SysReader::ram (s.ramUsedGb,  s.ramTotalGb,  s.ramPct);
    SysReader::disk(s.diskUsedGb, s.diskTotalGb, s.diskPct);
    s.cpuPct    = SysReader::cpu();
    s.uptime    = SysReader::uptime();
    s.processes = SysReader::processes();
    s.packages  = SysReader::packages();
    s.updates   = SysReader::updates();
    return s;
}
