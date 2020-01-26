#ifndef GAMEOPTION_H
#define GAMEOPTION_H

#include "gameaction.h"
#include <cost.h>
#include <gameoptiondata.h>

class GameRegistry;

class GameOption : public AH::Common::GameOptionData
{
public:
    GameOption(GameAction *action = NULL, AH::ContinueType cont = AH::CanContinue, AH::ChooseType choose = AH::ChooseOptional, AH::GamePhases phases = AH::NoGamePhase)
        : m_action(action)
    {
        m_chooseType = choose;
        m_continueType = cont;
        m_phases = phases;
        if (m_action && m_phases == AH::NoGamePhase) {
            m_phases = m_action->phases();
        }
    }
    virtual ~GameOption() {}

    enum class AutoChoose {
        Always,
        Never,
        Possible
    };

    void setId(QString id) { m_id = id; }

    GameOptionData *data();

    virtual AH::GamePhases phases() const;
    virtual AutoChoose autoChoose() const;

    //virtual bool isAvailable() const = 0;

    virtual bool execute();

    QString name() const;
    QString description() const;

    virtual bool resolveDependencies(GameRegistry *reg);

//    virtual bool disposeAfterExecute() { return false; }

    virtual GameAction *action() { return m_action; }
    virtual const GameAction *action() const { return m_action; }

protected:
    GameAction *m_action;
};

#endif // GAMEOPTION_H
