#pragma once
#include <QFrame>
#include <QLabel>
#include <QProgressBar>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "Theme.h"

class PanelWidget : public QFrame {
    Q_OBJECT
public:
    QLabel      *badge      = nullptr;
    QVBoxLayout *bodyLayout = nullptr;

    PanelWidget(const QString &title,
                const char    *dotColor,
                const QString &badgeText = QString(),
                QWidget       *parent    = nullptr);

    void clearBody();
};

class BarWidget : public QWidget {
    Q_OBJECT
    QLabel       *m_value;
    QProgressBar *m_bar;
public:
    explicit BarWidget(const QString &name, QWidget *parent = nullptr);
    void update(const QString &val, int pct);
};
