#ifndef CHOICEHELPER_H
#define CHOICEHELPER_H

#include <cost.h>

class Character;
class GameObject;

class ChoiceHelper
{
public:
    static bool choosePayment(Character *c, AH::Common::Cost options, AH::Common::CostList &selected);
    static void loseHalfPossesions(Character *c);

private:
    ChoiceHelper();
};

#endif // CHOICEHELPER_H
