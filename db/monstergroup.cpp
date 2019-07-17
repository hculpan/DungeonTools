#include "monstergroup.h"

MonsterGroup::MonsterGroup()
{

}

MonsterGroup::~MonsterGroup()
{
    for (Monster *m : monsters) {
        delete m;
    }
}

void MonsterGroup::setField(QString name, const QVariant *value)
{
    QString field = name.toLower();
    if (field == "id") {
        id = value->toInt();
    } else if (field == "name") {
        this->name = value->toString();
    } else if (field == "number") {
        number = value->toInt();
    } else if (field == "cr") {
        cr = value->toString();
    } else if (field == "xp") {
        xp = value->toInt();
    } else if (field == "hp") {
        hp = value->toInt();
    } else if (field == "attackbonus") {
        attackBonus = value->toInt();
    } else if (field == "damage") {
        damage = value->toInt();
    } else if (field == "savedc") {
        saveDc = value->toInt();
    } else if (field == "armorclass") {
        armorClass = value->toInt();
    }
}
