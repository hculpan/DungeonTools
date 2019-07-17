#include "encounter.h"

Encounter::Encounter()
{

}

Encounter::~Encounter()
{
    for (MonsterGroup *m : groups) {
//        delete m;
    }
}

void Encounter::setField(QString name, const QVariant *value)
{
    QString field = name.toLower();
    if (field == "id") {
        id = value->toInt();
    } else if (field == "sessionid") {
        sessionId = value->toInt();
    } else if (field == "totalxp") {
        totalXp = value->toInt();
    } else if (field == "monstercount") {
        monsterCount = value->toInt();
    } else if (field == "partysize") {
        partySize = value->toInt();
    } else if (field == "averagelevel") {
        averageLevel = value->toInt();
    } else if (field == "difficulty") {
        difficulty = value->toString();
    } else if (field == "started") {
        started = (value->toInt() > 0);
    } else if (field == "description") {
        description = value->toString();
    }
}

Monster *Encounter::getMonsterAt(int index)
{
    if (index >= monsterCount || index < 0) return nullptr;

    MonsterGroup *group = nullptr;
    int currentIndex = index;
    int groupIndex = 0;
    while (true) {
        group = groups[groupIndex];
        if (currentIndex - group->number < 0) {
            break;
        } else {
            currentIndex -= group->number;
        }
        groupIndex++;
    }

    return group->monsters[currentIndex];
}

MonsterGroup *Encounter::getMonsterGroupFor(Monster *m)
{
    MonsterGroup *result = nullptr;
    for(MonsterGroup * g : groups) {
        if (g->id == m->groupId) {
            result = g;
            break;
        }
    }
    return result;
}
