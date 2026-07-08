#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Basic application metadata (useful for QSettings, About dialogs, etc.
    // in a future, non-UI-only iteration).
    QApplication::setApplicationName(QStringLiteral("Expense Tracker"));
    QApplication::setOrganizationName(QStringLiteral("ExampleOrg"));

    MainWindow window;
    window.show();

    return app.exec();
}
