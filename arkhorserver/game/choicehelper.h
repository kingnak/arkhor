#ifndef CHOICEHELPER_H
#define CHOICEHELPER_H

#include <cost.h>

class Character;
class GameObject;

class ChoiceHelper
{
public:
    static bool choosePayment(Character *c, AH::Common::Cost options, AH::Common::CostList &selected, const QString &sourceId, const QString &description);
    static void loseHalfPossesions(Character *c, const QString &sourceId = QString::null);
    static void losePossessions(Character *c, int count, const QString &sourceId = QString::null);

private:
    ChoiceHelper();
};

#endif // CHOICEHELPER_H
