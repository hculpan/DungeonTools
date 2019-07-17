#include "settings.h"

Settings *settings;

#include <QDebug>

Settings::Settings(QObject *parent) : QObject(parent)
{
    QSettings settings(QDir::homePath() + QDir::separator() + ".dungeontools" + QDir::separator() + "dungeontools.ini", QSettings::IniFormat);

    settings.beginGroup("MainWindow");
    this->mainWindowX = settings.value("x").toInt();
    this->mainWindowY = settings.value("y").toInt();
    settings.endGroup();

    qDebug() << settings.fileName();

    settings.beginGroup("db");
    QString path = settings.value("path").toString();
    if (path.isEmpty()) {
        dbPath.setFile(QDir::homePath(), ".dungeontools" + QString(QDir::separator()) + "dungeontools-db.sqlite");
    } else {
        dbPath.setFile(path);
    }
#ifdef QT_DEBUG
    QString file = "", ext = "";
    int index = dbPath.fileName().lastIndexOf('.');
    if (index == -1) {
        file = dbPath.fileName();
    } else {
        file = dbPath.fileName().mid(0, index);
        ext = dbPath.fileName().right(dbPath.fileName().length() - (index + 1));
    }
    dbPath.setFile(dbPath.absolutePath(), file + "-dev." + ext);
#endif
    settings.endGroup();

    settings.beginGroup("encounters");
    currentEncounterId = settings.value("current").toInt();
    settings.endGroup();
}

void Settings::writeSettings() {
    QSettings settings(QDir::homePath() + QDir::separator() + ".dungeontools" + QDir::separator() + "dungeontools.ini", QSettings::IniFormat);

    settings.beginGroup("MainWindow");
    settings.setValue("x", mainWindowX);
    settings.setValue("y", mainWindowY);
    settings.endGroup();

    settings.beginGroup("db");
#ifdef QT_DEBUG
    settings.setValue("path", dbPath.absoluteFilePath().remove("-dev"));
#else
    settings.setValue("path", dbPath.absoluteFilePath());
#endif
    settings.endGroup();

    settings.beginGroup("encounters");
    settings.setValue("current", currentEncounterId);
    settings.endGroup();

    settings.sync();
}
