#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QSettings>
#include <QFileInfo>
#include <QDir>

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(QObject *parent = nullptr);

    void writeSettings();

    int mainWindowX;
    int mainWindowY;

    QFileInfo dbPath;

    int currentEncounterId;

signals:

public slots:
};

extern Settings *settings;

#endif // SETTINGS_H
