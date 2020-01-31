#ifndef GAMEINTERACTOR_H
#define GAMEINTERACTOR_H

#include <QList>
#include "attributeslider.h"
#include "movementpath.h"
#include "propertyvalue.h"
#include <cost.h>
#include <diedata.h>
#include "propertymodifier.h"
#include "encounterdata.h"
#include <choicedata.h>
#include <monsterdata.h>
#include <tradedata.h>

class Game;
class GameOption;
class GameObject;
class Investigator;
class Player;

class GameInteractor
{
public:
    GameInteractor() {}
    virtual ~GameInteractor() {}

    virtual void init(Game *game) = 0;

    // Preparation
    virtual Investigator *chooseInvestigator(QList<Investigator *> invs) = 0;
    virtual void playerCharacterInstantiated(Player *p) = 0;

    // Main Game
    virtual GameOption *chooseOption(QList<GameOption *> options) = 0;
    virtual QList<int> chooseFocus(QList<AttributeSlider> sliders, int totalFocus) = 0;
    virtual MovementPath chooseMovement(GameField *start, int movement) = 0;
    virtual QString chooseMonster(QString desc, QList<AH::Common::MonsterData> monsters) = 0;

    //virtual AH::Common::CostList choosePayment(const AH::Common::Cost &c) = 0;

    virtual QString chooseEncounterOption(AH::Common::EncounterData *enc) = 0;

    virtual bool chooseWeapons(QList<GameObject *> weapons, ModifiedPropertyValue hands, QStringList &selected) = 0;

    virtual AH::Common::PropertyValueData::Property chooseSkill(QList<AH::Common::ModifiedPropertyValueData> options) = 0;

    virtual AH::Common::DieTestUpdateData dieRollStart(const AH::Common::DieRollTestData &test) = 0;
    virtual AH::Common::DieTestUpdateData dieRollUpdate(const AH::Common::DieRollTestData &test) = 0;
    virtual void dieRollFinish(const AH::Common::DieRollTestData &test) = 0;

    virtual AH::Common::ChoiceResponseData offerChoice(AH::Common::ChoiceData choice) = 0;

    virtual AH::Common::TradeData offerTrade(AH::Common::TradeData trade) = 0;
    virtual void tradeCanceled(QString name) = 0;
};

#endif // GAMEINTERACTOR_H
