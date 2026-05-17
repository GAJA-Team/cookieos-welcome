#include "WelcomeWindow.h"
#include "OsRelease.h"
#include "Data.h"
#include "Theme.h"

#include <QApplication>
#include <QGuiApplication>
#include <QScreen>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QPushButton>
#include <QCheckBox>
#include <QFrame>
#include <QDateTime>
#include <QPainter>
#include <QPixmap>
#include <QRegularExpression>
#include <QDesktopServices>
#include <QUrl>

static QPushButton* linkBtn(const QString &icon, const QString &label,
                            const QString &sub,  const char *accentColor)
{
    auto *btn = new QPushButton;
    btn->setCursor(Qt::PointingHandCursor);
    btn->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    btn->setFixedHeight(68);
    btn->setStyleSheet(QString(
        "QPushButton { background:%1; border:1px solid %2; border-radius:10px; padding:0; }"
        "QPushButton:hover { border-color:%3; background:#1e1e26; }"
        "QPushButton:pressed { background:%4; }")
    .arg(Th::BG3, Th::BORDER, accentColor, Th::BG));

    auto *lay = new QHBoxLayout(btn);
    lay->setContentsMargins(16,0,16,0);
    lay->setSpacing(14);

    auto *ico = new QLabel(icon);
    ico->setStyleSheet(QString("font-size:22pt; background:transparent; color:%1;").arg(accentColor));
    ico->setFixedWidth(34);
    ico->setAttribute(Qt::WA_TransparentForMouseEvents);
    lay->addWidget(ico);

    auto *col = new QVBoxLayout;
    col->setSpacing(2);
    auto *lbl = new QLabel(label);
    lbl->setStyleSheet(QString("font-size:10pt; font-weight:700; color:%1; background:transparent;").arg(accentColor));
    lbl->setAttribute(Qt::WA_TransparentForMouseEvents);
    col->addWidget(lbl);
    auto *sublbl = new QLabel(sub);
    sublbl->setStyleSheet(QString("font-size:8pt; color:%1; background:transparent;").arg(Th::MUTED));
    sublbl->setAttribute(Qt::WA_TransparentForMouseEvents);
    col->addWidget(sublbl);
    lay->addLayout(col);
    lay->addStretch();

    auto *arrow = new QLabel("→");
    arrow->setStyleSheet(QString("font-size:14pt; color:%1; background:transparent;").arg(Th::BORDER));
    arrow->setAttribute(Qt::WA_TransparentForMouseEvents);
    lay->addWidget(arrow);

    return btn;
}

WelcomeWindow::WelcomeWindow(QWidget *parent) : QMainWindow(parent)
{
    setWindowTitle("CookieOS — Welcome");
    setMinimumSize(980, 680);
    resize(1020, 740);
    setStyleSheet(QString("QMainWindow { background:%1; }").arg(Th::BG));

    auto *scroll = new QScrollArea;
    scroll->setWidgetResizable(true);
    scroll->setFrameShape(QFrame::NoFrame);
    setCentralWidget(scroll);

    auto *central = new QWidget;
    central->setStyleSheet(QString("background:%1;").arg(Th::BG));
    scroll->setWidget(central);

    auto *root = new QVBoxLayout(central);
    root->setContentsMargins(28,22,28,30);
    root->setSpacing(16);

    root->addWidget(buildHeader());
    root->addWidget(Th::hLine());

    auto *grid = new QGridLayout;
    grid->setSpacing(12);
    grid->setColumnStretch(0,1);
    grid->setColumnStretch(1,1);
    grid->setColumnStretch(2,1);
    grid->addWidget(buildLinksPanel(),     0, 0);
    grid->addWidget(buildChangelogPanel(), 0, 1);
    grid->addWidget(buildAppsPanel(),      0, 2);
    grid->addWidget(buildSysPanel(),       1, 0, 1, 3);
    root->addLayout(grid);

    root->addWidget(buildTipsPanel());
    root->addWidget(Th::hLine());
    root->addWidget(buildFooter());

    OsRelease os = parseOsRelease();
    static QRegularExpression reOS(R"(^(\w+?)(OS)(.*))", QRegularExpression::CaseInsensitiveOption);
    auto mtch = reOS.match(os.prettyName);
    if (mtch.hasMatch())
        m_osName->setText(mtch.captured(1) +
            QString("<span style='color:%1;'>%2</span>").arg(Th::ACCENT, mtch.captured(2)) +
            mtch.captured(3));
    else
        m_osName->setText(os.prettyName);
    m_osSub->setText(QString("$ uname -a → %1%2").arg(os.prettyName)
        .arg(os.versionCodename.isEmpty() ? "" : " (" + os.versionCodename + ")"));
    m_version->setText("v" + os.versionId + "-stable");

    m_clockTimer = new QTimer(this);
    connect(m_clockTimer, &QTimer::timeout, this, &WelcomeWindow::tick);
    m_clockTimer->start(1000);
    tick();

    m_sysTimer = new QTimer(this);
    connect(m_sysTimer, &QTimer::timeout, this, &WelcomeWindow::refreshSys);
    m_sysTimer->start(30000);
    refreshSys();
}

void WelcomeWindow::tick()
{
    if (m_clock)
        m_clock->setText(QDateTime::currentDateTime().toString("ddd dd.MM  hh:mm:ss"));
}

void WelcomeWindow::refreshSys() { applySysInfo(collectSysInfo()); }

QWidget* WelcomeWindow::buildHeader()
{
    auto *w = new QWidget;
    auto *lay = new QHBoxLayout(w);
    lay->setContentsMargins(0,0,0,0);
    lay->setSpacing(16);

    QPixmap px(50,50);
    px.fill(Qt::transparent);
    {
        QPainter p(&px);
        p.setRenderHint(QPainter::Antialiasing);
        p.setPen(QPen(QColor(Th::ACCENT),1.5));
        p.setBrush(QColor(245,166,35,30));
        p.drawEllipse(2,2,46,46);
        p.setPen(Qt::NoPen);
        p.setBrush(QColor(245,166,35,45));
        p.drawEllipse(7,7,36,36);
        p.setBrush(QColor("#e67e22"));
        p.drawEllipse(10,10,30,30);
        p.setBrush(QColor(Th::BG));
        struct C{int x,y,d;};
        for(auto[x,y,d]:QList<C>{{17,18,5},{27,21,4},{20,28,4},{29,28,3},{13,25,3},{31,14,3}})
            p.drawEllipse(x,y,d,d);
    }
    auto *logo = new QLabel; logo->setPixmap(px); logo->setFixedSize(50,50);
    lay->addWidget(logo);

    auto *col = new QVBoxLayout; col->setSpacing(3);
    m_osName = new QLabel;
    m_osName->setTextFormat(Qt::RichText);
    m_osName->setStyleSheet("font-size:20pt; font-weight:800; letter-spacing:-0.5px; color:#ffffff; background:transparent;");
    col->addWidget(m_osName);
    m_osSub = Th::mono("$ ładowanie...", Th::MUTED, 8);
    col->addWidget(m_osSub);
    lay->addLayout(col);
    lay->addStretch();

    auto *rcol = new QVBoxLayout; rcol->setSpacing(5); rcol->setAlignment(Qt::AlignRight|Qt::AlignVCenter);
    m_version = new QLabel("v—"); m_version->setAlignment(Qt::AlignCenter);
    m_version->setStyleSheet(QString("background:%1; border:1px solid %2; border-radius:12px;"
    "padding:3px 14px; font-family:'JetBrains Mono',monospace; font-size:8pt; color:%3;")
    .arg(Th::BG3, Th::BORDER, Th::ACCENT));
    rcol->addWidget(m_version, 0, Qt::AlignRight);
    m_clock = Th::mono("", Th::MUTED, 8); m_clock->setAlignment(Qt::AlignRight);
    rcol->addWidget(m_clock, 0, Qt::AlignRight);
    lay->addLayout(rcol);
    return w;
}

QWidget* WelcomeWindow::buildLinksPanel()
{
    auto *panel = new PanelWidget("CookieOS online", Th::ACCENT, "2 links");

    auto *btnBlog = linkBtn("📝", "CookieOS Blog", "", Th::ACCENT);
    connect(btnBlog, &QPushButton::clicked, []{ QDesktopServices::openUrl(QUrl("https://gaja-team.github.io/CookieOS/blog")); });
    panel->bodyLayout->addWidget(btnBlog);

    panel->bodyLayout->addSpacing(8);

    auto *btnHome = linkBtn("🌐", "CookieOS Website", "gaja-team.github.io/CookieOS", Th::BLUE);
    connect(btnHome, &QPushButton::clicked, []{ QDesktopServices::openUrl(QUrl("https://gaja-team.github.io/CookieOS")); });
    panel->bodyLayout->addWidget(btnHome);

    panel->bodyLayout->addStretch();
    return panel;
}

QWidget* WelcomeWindow::buildChangelogPanel()
{
    OsRelease os = parseOsRelease();
    auto *panel = new PanelWidget("Changes in OS", Th::BLUE, "v" + os.versionId);
    bool first = true;
    for (auto &item : changelogData()) {
        if (!first) panel->bodyLayout->addWidget(Th::hLine());
        first = false;
        auto *row = new QHBoxLayout; row->setContentsMargins(0,5,0,5); row->setSpacing(8); row->setAlignment(Qt::AlignTop);
        row->addWidget(Th::clIcon(item.type), 0, Qt::AlignTop);
        auto *col = new QVBoxLayout; col->setSpacing(2);
        auto *txt = new QLabel(item.text); txt->setWordWrap(true);
        txt->setStyleSheet(QString("font-size:9pt; color:%1; background:transparent;").arg(Th::TEXT));
        col->addWidget(txt);
        col->addWidget(Th::mono(item.ver, Th::MUTED, 7));
        row->addLayout(col);
        panel->bodyLayout->addLayout(row);
    }
    panel->bodyLayout->addStretch();
    return panel;
}

QWidget* WelcomeWindow::buildAppsPanel()
{
    auto apps = appsData();
    auto *panel = new PanelWidget("CookieOS Apps", Th::GREEN, QString("%1 apps").arg(apps.size()));
    auto *grid = new QGridLayout; grid->setSpacing(7);
    int col = 0, row = 0;
    for (auto &app : apps) {
        auto *tile = new QFrame; tile->setObjectName("AppTile"); tile->setFixedHeight(80);
        tile->setStyleSheet(QString("QFrame#AppTile { background:%1; border:1px solid %2; border-radius:8px; }"
        "QFrame#AppTile:hover { border-color:%3; background:#1e1e24; }").arg(Th::BG3, Th::BORDER, Th::ACCENT));
        tile->setCursor(Qt::PointingHandCursor);
        auto *tl = new QVBoxLayout(tile); tl->setContentsMargins(4,7,4,7); tl->setSpacing(3); tl->setAlignment(Qt::AlignCenter);
        auto *ico = new QLabel(app.icon); ico->setAlignment(Qt::AlignCenter); ico->setFixedSize(32,32);
        ico->setStyleSheet(QString("background:%1; border-radius:6px; font-size:16pt; padding:2px;").arg(app.color));
        tl->addWidget(ico, 0, Qt::AlignCenter);
        auto *nm = new QLabel(app.name); nm->setAlignment(Qt::AlignCenter);
        nm->setStyleSheet("font-size:7pt; font-weight:700; background:transparent;");
        tl->addWidget(nm);
        auto *ds = new QLabel(app.desc); ds->setAlignment(Qt::AlignCenter); ds->setWordWrap(true);
        ds->setStyleSheet(QString("font-size:6pt; color:%1; background:transparent;").arg(Th::MUTED));
        tl->addWidget(ds);
        grid->addWidget(tile, row, col);
        if (++col == 2) { col = 0; ++row; }
    }
    panel->bodyLayout->addLayout(grid);
    panel->bodyLayout->addStretch();
    return panel;
}

QWidget* WelcomeWindow::buildSysPanel()
{
    auto *panel = new PanelWidget("system status", Th::MUTED, "live");
    auto *hlay = new QHBoxLayout; hlay->setSpacing(24);

    auto *leftCol = new QVBoxLayout; leftCol->setSpacing(0);
    auto mkRow = [&](const QString &key, QLabel *&ref, const char *initColor = Th::TEXT) {
        auto *row = new QHBoxLayout; row->setContentsMargins(0,3,0,3);
        row->addWidget(Th::mono(key, Th::MUTED, 8)); row->addStretch();
        ref = new QLabel("—");
        ref->setStyleSheet(QString("font-family:'JetBrains Mono',monospace; font-size:9pt;"
        "font-weight:600; color:%1; background:transparent;").arg(initColor));
        row->addWidget(ref);
        leftCol->addLayout(row);
        leftCol->addWidget(Th::hLine());
    };
    mkRow("kernel", m_kernelVal);
    {
        auto *row = new QHBoxLayout; row->setContentsMargins(0,3,0,3);
        row->addWidget(Th::mono("init", Th::MUTED, 8)); row->addStretch();
        auto *v = new QLabel("systemd ✓");
        v->setStyleSheet(QString("font-family:'JetBrains Mono',monospace; font-size:9pt; font-weight:600; color:%1; background:transparent;").arg(Th::GREEN));
        row->addWidget(v); leftCol->addLayout(row); leftCol->addWidget(Th::hLine());
    }
    mkRow("pakiets",      m_pkgVal);
    mkRow("process",      m_procVal);
    mkRow("uptime",       m_uptimeVal,  Th::GREEN);
    mkRow("updates", m_updatesVal, Th::GREEN);
    leftCol->addStretch();

    auto *rightCol = new QVBoxLayout; rightCol->setSpacing(0); rightCol->addStretch();
    m_ramBar  = new BarWidget("RAM");
    m_diskBar = new BarWidget("Disk /");
    m_cpuBar  = new BarWidget("CPU");
    rightCol->addWidget(m_ramBar);
    rightCol->addWidget(m_diskBar);
    rightCol->addWidget(m_cpuBar);
    rightCol->addStretch();

    hlay->addLayout(leftCol, 1);
    auto *vline = new QFrame; vline->setFrameShape(QFrame::VLine);
    vline->setStyleSheet(QString("border:none; border-left:1px solid %1;").arg(Th::BORDER));
    hlay->addWidget(vline);
    hlay->addLayout(rightCol, 1);
    panel->bodyLayout->addLayout(hlay);
    return panel;
}

void WelcomeWindow::applySysInfo(const SysInfo &s)
{
    if (m_kernelVal)  m_kernelVal->setText(s.kernel);
    if (m_pkgVal)     m_pkgVal->setText(s.packages);
    if (m_procVal)    m_procVal->setText(QString::number(s.processes));
    if (m_uptimeVal)  m_uptimeVal->setText(s.uptime);
    if (m_updatesVal) {
        if (s.updates > 0) {
            m_updatesVal->setText(QString("%1 dostępne").arg(s.updates));
            m_updatesVal->setStyleSheet(QString("font-family:'JetBrains Mono',monospace; font-size:9pt; font-weight:600; color:%1; background:transparent;").arg(Th::ACCENT));
        } else {
            m_updatesVal->setText("aktualny ✓");
            m_updatesVal->setStyleSheet(QString("font-family:'JetBrains Mono',monospace; font-size:9pt; font-weight:600; color:%1; background:transparent;").arg(Th::GREEN));
        }
    }
    if (m_ramBar)  m_ramBar->update(QString("%1 / %2 GB").arg(s.ramUsedGb,0,'f',1).arg(s.ramTotalGb,0,'f',1), (int)s.ramPct);
    if (m_diskBar) m_diskBar->update(QString("%1 / %2 GB").arg(s.diskUsedGb,0,'f',1).arg(s.diskTotalGb,0,'f',1), (int)s.diskPct);
    if (m_cpuBar)  m_cpuBar->update(QString("%1%").arg((int)s.cpuPct), (int)s.cpuPct);
}

QWidget* WelcomeWindow::buildTipsPanel()
{
    auto *panel = new PanelWidget("Tips", Th::PURPLE, "from creators");
    struct Tip { QString icon, html; };
    QList<Tip> tips{
    };
    auto *grid = new QGridLayout; grid->setSpacing(8);
    int col = 0, row = 0;
    for (auto &tip : tips) {
        auto *tile = new QFrame; tile->setObjectName("Tip");
        tile->setStyleSheet(QString("QFrame#Tip { background:%1; border:1px solid %2; border-radius:8px; }").arg(Th::BG3, Th::BORDER));
        auto *tl = new QHBoxLayout(tile); tl->setContentsMargins(10,8,10,8); tl->setSpacing(10);
        auto *ico = new QLabel(tip.icon); ico->setStyleSheet("font-size:12pt; background:transparent;"); ico->setFixedWidth(22);
        tl->addWidget(ico, 0, Qt::AlignTop);
        auto *txt = new QLabel(tip.html); txt->setTextFormat(Qt::RichText); txt->setWordWrap(true);
        txt->setStyleSheet(QString("font-size:9pt; color:%1; background:transparent;").arg(Th::TEXT));
        tl->addWidget(txt);
        grid->addWidget(tile, row, col);
        if (++col == 2) { col = 0; ++row; }
    }
    panel->bodyLayout->addLayout(grid);
    return panel;
}

QWidget* WelcomeWindow::buildFooter()
{
    auto *w = new QWidget;
    auto *lay = new QHBoxLayout(w); lay->setContentsMargins(0,4,0,0); lay->setSpacing(12);
    auto *copy = new QLabel(QString("CookieOS © <span style='color:%1;'>2026</span> &mdash; build with <span style='color:%1;'>♥</span> on Debian &middot; GPL-3.0").arg(Th::ACCENT));
    copy->setTextFormat(Qt::RichText);
    copy->setStyleSheet(QString("font-family:'JetBrains Mono',monospace; font-size:8pt; color:%1;").arg(Th::MUTED));
    lay->addWidget(copy); lay->addStretch();
    auto *chk = new QCheckBox("Don't show at startup");
    chk->setStyleSheet(QString("QCheckBox { color:%1; font-size:9pt; spacing:6px; }"
    "QCheckBox::indicator { width:13px; height:13px; border:1px solid %2; border-radius:3px; background:%3; }"
    "QCheckBox::indicator:checked { background:%4; border-color:%4; }").arg(Th::MUTED, Th::BORDER, Th::BG3, Th::ACCENT));
    lay->addWidget(chk);
    auto *btn = new QPushButton("  Close  ");
    btn->setStyleSheet(QString("QPushButton { background:%1; border:1px solid %2; color:%3;"
    "font-family:'Syne',sans-serif; font-size:9pt; font-weight:700; padding:6px 18px; border-radius:6px; }"
    "QPushButton:hover { border-color:%4; color:%4; background:%1; }"
    "QPushButton:pressed { background:%5; }").arg(Th::BG3, Th::BORDER, Th::TEXT, Th::ACCENT, Th::BG));
    connect(btn, &QPushButton::clicked, this, &QMainWindow::close);
    lay->addWidget(btn);
    return w;
}
