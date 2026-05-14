#include "Panels.h"
#include "Theme.h"

// ── PanelWidget ──────────────────────────────────────────────────────────────
PanelWidget::PanelWidget(const QString &title,
                         const char    *dotColor,
                         const QString &badgeText,
                         QWidget       *parent)
    : QFrame(parent)
{
    setObjectName("Panel");
    setStyleSheet(QString(
        "QFrame#Panel { background:%1; border:1px solid %2; border-radius:10px; }")
        .arg(Th::BG2, Th::BORDER));

    auto *root = new QVBoxLayout(this);
    root->setContentsMargins(0,0,0,0);
    root->setSpacing(0);

    // nagłówek
    auto *hdr = new QWidget;
    hdr->setObjectName("PanelHdr");
    hdr->setStyleSheet(QString(
        "QWidget#PanelHdr {"
        "  background:%1;"
        "  border-top-left-radius:10px; border-top-right-radius:10px;"
        "  border-bottom:1px solid %2; }").arg(Th::BG3, Th::BORDER));
    auto *hl = new QHBoxLayout(hdr);
    hl->setContentsMargins(12,8,12,8);
    hl->setSpacing(8);

    auto *dot = new QLabel;
    dot->setFixedSize(8,8);
    dot->setStyleSheet(QString("background:%1; border-radius:4px;").arg(dotColor));
    hl->addWidget(dot);

    auto *ttl = new QLabel(title.toUpper());
    ttl->setStyleSheet(QString(
        "font-weight:700; font-size:8pt; letter-spacing:1px;"
        "color:%1; background:transparent;").arg(Th::TEXT));
    hl->addWidget(ttl);
    hl->addStretch();

    if (!badgeText.isEmpty()) {
        badge = new QLabel(badgeText);
        badge->setStyleSheet(QString(
            "background:%1; border:1px solid %2; border-radius:8px;"
            "padding:1px 8px; font-family:'JetBrains Mono',monospace;"
            "font-size:7pt; color:%3;").arg(Th::BG, Th::BORDER, Th::MUTED));
        hl->addWidget(badge);
    }
    root->addWidget(hdr);

    // ciało
    auto *body = new QWidget;
    bodyLayout = new QVBoxLayout(body);
    bodyLayout->setContentsMargins(14,12,14,12);
    bodyLayout->setSpacing(0);
    root->addWidget(body);
}

void PanelWidget::clearBody()
{
    while (QLayoutItem *it = bodyLayout->takeAt(0)) {
        if (it->widget()) it->widget()->deleteLater();
        delete it;
    }
}

// ── BarWidget ────────────────────────────────────────────────────────────────
BarWidget::BarWidget(const QString &name, QWidget *parent)
    : QWidget(parent)
{
    auto *lay = new QVBoxLayout(this);
    lay->setContentsMargins(0,6,0,2);
    lay->setSpacing(2);

    auto *row = new QHBoxLayout;
    row->addWidget(Th::mono(name));
    row->addStretch();
    m_value = Th::mono("—", Th::TEXT);
    row->addWidget(m_value);
    lay->addLayout(row);

    m_bar = new QProgressBar;
    m_bar->setFixedHeight(4);
    m_bar->setTextVisible(false);
    m_bar->setRange(0,100);
    m_bar->setValue(0);
    m_bar->setStyleSheet(QString(
        "QProgressBar { background:%1; border:none; border-radius:2px; }"
        "QProgressBar::chunk { background:%2; border-radius:2px; }")
        .arg(Th::BG3, Th::GREEN));
    lay->addWidget(m_bar);
}

void BarWidget::update(const QString &val, int pct)
{
    m_value->setText(val);
    m_bar->setValue(pct);
    const char *color = pct < 60 ? Th::GREEN : pct < 80 ? Th::ACCENT : Th::RED;
    m_bar->setStyleSheet(QString(
        "QProgressBar { background:%1; border:none; border-radius:2px; }"
        "QProgressBar::chunk { background:%2; border-radius:2px; }")
        .arg(Th::BG3, color));
}
