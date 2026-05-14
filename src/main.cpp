#include <QApplication>
#include <QScreen>
#include "WelcomeWindow.h"
#include "Theme.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("CookieOS Welcome");
    app.setApplicationVersion("1.0");
    app.setOrganizationName("CookieOS");
    app.setStyleSheet(Th::globalQss());

    WelcomeWindow w;

    if (auto *screen = QGuiApplication::primaryScreen()) {
        QRect sg = screen->availableGeometry();
        w.move(sg.center() - w.rect().center());
    }

    w.show();
    return app.exec();
}
