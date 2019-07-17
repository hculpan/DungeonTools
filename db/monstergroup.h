#ifndef MONSTERGROUP_H
#define MONSTERGROUP_H

#include <QString>
#include <QVariant>

#include "db/monster.h"

class MonsterGroup
{
public:
    MonsterGroup();
    ~MonsterGroup();

    int id;
    QString name;
    int number;
    QString cr;
    int xp;
    int hp;
    int attackBonus;
    int damage;
    int saveDc;
    int armorClass;

    void setField(QString name, const QVariant *value);

    QList<Monster *> monsters;

    static int columnCount() { return 9; }
};

#endif // MONSTERGROUP_H
