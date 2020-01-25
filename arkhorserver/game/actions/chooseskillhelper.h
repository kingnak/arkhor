#ifndef CHOOSESKILLHELPER_H
#define CHOOSESKILLHELPER_H

#include <ahglobal.h>

class Player;

class ChooseSkillHelper
{
public:
    static AH::Skill chooseSkill(Player *p, QList<AH::Skill> options);
    static AH::Skill chooseSkillDieRoll(Player *p, QList<AH::Skill> options);

private:
    ChooseSkillHelper();
    Q_DISABLE_COPY(ChooseSkillHelper)
};

#endif // CHOOSESKILLHELPER_H
