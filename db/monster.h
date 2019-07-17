#ifndef MONSTER_H
#define MONSTER_H

#include <QString>
#include <QVariant>

class Monster
{
public:
    Monster();
    ~Monster();

    int id;
    int number;
    int groupId;
    int health;

    void setField(QString name, const QVariant *value);
};

#endif // MONSTER_H
