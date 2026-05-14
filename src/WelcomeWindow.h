#pragma once
#include <QMainWindow>
#include <QLabel>
#include <QTimer>
#include "Panels.h"
#include "SysInfo.h"

class WelcomeWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit WelcomeWindow(QWidget *parent = nullptr);

private slots:
    void tick();
    void refreshSys();

private:
    QWidget* buildHeader();
    QWidget* buildLinksPanel();
    QWidget* buildChangelogPanel();
    QWidget* buildAppsPanel();
    QWidget* buildSysPanel();
    QWidget* buildTipsPanel();
    QWidget* buildFooter();

    void applySysInfo(const SysInfo &s);

    // header
    QLabel *m_osName  = nullptr;
    QLabel *m_osSub   = nullptr;
    QLabel *m_version = nullptr;
    QLabel *m_clock   = nullptr;

    // sys panel
    QLabel    *m_kernelVal  = nullptr;
    QLabel    *m_pkgVal     = nullptr;
    QLabel    *m_procVal    = nullptr;
    QLabel    *m_uptimeVal  = nullptr;
    QLabel    *m_updatesVal = nullptr;
    BarWidget *m_ramBar     = nullptr;
    BarWidget *m_diskBar    = nullptr;
    BarWidget *m_cpuBar     = nullptr;

    QTimer *m_clockTimer = nullptr;
    QTimer *m_sysTimer   = nullptr;

    static constexpr auto URL_BLOG = "https://gaja-team.github.io/CookieOS/blog";
    static constexpr auto URL_HOME = "https://gaja-team.github.io/CookieOS";
};
