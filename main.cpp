#include "mainwindow.h"
#include <QApplication>

#include "settings.h"
#include "db/db.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QApplication::setOrganizationDomain("org.culpan");
    QApplication::setApplicationName("DungeonTools");

    a.setStyleSheet(
        "QTableView {"
            "font-size: 12pt;"
            "gridline-color: gray;"
        "}"
        "QHeaderView {"
            "font-size: 10pt;"
            "border-color: black"
        "}"
    );

    settings = new Settings(&a);

    db = new Db(settings->dbPath.absoluteFilePath(), &a);

    MainWindow w;
    w.show();

    return a.exec();
}
