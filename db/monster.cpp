#include "monster.h"

Monster::Monster()
{

}

Monster::~Monster()
{
}

void Monster::setField(QString name, const QVariant *value)
{
    QString field = name.toLower();
    if (field == "id") {
        id = value->toInt();
    } else if (field == "number") {
        number = value->toInt();
    } else if (field == "groupid") {
        groupId = value->toInt();
    } else if (field == "health") {
        health = value->toInt();
    }
}
