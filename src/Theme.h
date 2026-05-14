#pragma once
#include <QString>
#include <QLabel>
#include <QFrame>
#include <QColor>

namespace Th {
    // ── Kolory ─────────────────────────────────────────────────────────────
    constexpr auto BG      = "#0d0d0f";
    constexpr auto BG2     = "#141418";
    constexpr auto BG3     = "#1a1a20";
    constexpr auto BORDER  = "#2a2a35";
    constexpr auto ACCENT  = "#f5a623";
    constexpr auto TEXT    = "#e8e8f0";
    constexpr auto MUTED   = "#7a7a90";
    constexpr auto GREEN   = "#4ade80";
    constexpr auto BLUE    = "#60a5fa";
    constexpr auto RED     = "#f87171";
    constexpr auto PURPLE  = "#a78bfa";

    // ── Global stylesheet ───────────────────────────────────────────────────
    inline QString globalQss()
    {
        return QString(
            "* { box-sizing: border-box; }"
            "QWidget          { background:%1; color:%2; }"
            "QLabel           { background:transparent; color:%2; }"
            "QScrollArea      { border:none; }"
            "QScrollBar:vertical { background:%3; width:5px; border-radius:2px; }"
            "QScrollBar::handle:vertical { background:%4; border-radius:2px; min-height:20px; }"
            "QScrollBar::add-line:vertical, QScrollBar::sub-line:vertical { height:0; }"
            "QToolTip         { background:%3; color:%2; border:1px solid %4; font-size:9pt; }"
        ).arg(BG, TEXT, BG3, BORDER);
    }

    // ── Horizontal divider ──────────────────────────────────────────────────
    inline QFrame* hLine(QWidget *parent = nullptr)
    {
        auto *f = new QFrame(parent);
        f->setFrameShape(QFrame::HLine);
        f->setStyleSheet(QString("border:none; border-top:1px solid %1; background:transparent;").arg(BORDER));
        f->setFixedHeight(1);
        return f;
    }

    // ── Monospaced label ────────────────────────────────────────────────────
    inline QLabel* mono(const QString &text,
                        const char   *color  = MUTED,
                        int           ptSize = 8,
                        QWidget      *parent = nullptr)
    {
        auto *l = new QLabel(text, parent);
        l->setStyleSheet(QString(
            "font-family:'JetBrains Mono',monospace; font-size:%1pt; color:%2;")
            .arg(ptSize).arg(color));
        return l;
    }

    // ── Tag badge ───────────────────────────────────────────────────────────
    inline QLabel* tag(const QString &t, QWidget *parent = nullptr)
    {
        struct S { const char *bg, *fg, *border; };
        static const QMap<QString,S> M {
            {"release",  {"#1a2e1a", GREEN,  "#2a4a2a"}},
            {"update",   {"#1a1a2e", BLUE,   "#2a2a4a"}},
            {"security", {"#2e1a1a", RED,    "#4a2a2a"}},
            {"news",     {"#2e2a1a", ACCENT, "#4a3a1a"}},
        };
        auto s = M.value(t, {"#222230", MUTED, BORDER});
        auto *l = new QLabel(t, parent);
        l->setStyleSheet(QString(
            "background:%1; color:%2; border:1px solid %3;"
            "border-radius:8px; padding:1px 7px;"
            "font-family:'JetBrains Mono',monospace; font-size:7pt; font-weight:600;")
            .arg(s.bg, s.fg, s.border));
        return l;
    }

    // ── Changelog type icon ─────────────────────────────────────────────────
    inline QLabel* clIcon(const QString &type, QWidget *parent = nullptr)
    {
        struct S { const char *bg, *fg, *sym; };
        static const QMap<QString,S> M {
            {"add",   {"#1a2e1a", GREEN,  "+"}},
            {"fix",   {"#1a1a2e", BLUE,   "~"}},
            {"break", {"#2e1a1a", RED,    "!"}},
            {"perf",  {"#2e2a1a", ACCENT, "↑"}},
        };
        auto s = M.value(type, {"#222230", MUTED, "·"});
        auto *l = new QLabel(s.sym, parent);
        l->setFixedSize(20, 20);
        l->setAlignment(Qt::AlignCenter);
        l->setStyleSheet(QString(
            "background:%1; color:%2; border-radius:5px;"
            "font-weight:700; font-size:10pt;").arg(s.bg, s.fg));
        return l;
    }
} // namespace Th
