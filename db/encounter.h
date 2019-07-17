#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include <QString>
#include <QList>

#include "db/monstergroup.h"

class Encounter
{
public:
    Encounter();
    ~Encounter();

    int id;
    int sessionId;
    int totalXp;
    int monsterCount;
    int partySize;
    int averageLevel;
    QString difficulty;
    bool started;
    QString description;

    QList<MonsterGroup *> groups;

    void setField(QString name, const QVariant *value);

    Monster *getMonsterAt(int index);
    MonsterGroup *getMonsterGroupFor(Monster *m);
};

#endif // ENCOUNTER_H
