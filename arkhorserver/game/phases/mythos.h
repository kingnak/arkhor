#ifndef MYTHOS_H
#define MYTHOS_H

#include "game/gamephase.h"
#include "game/game.h"
#include "game/gameaction.h"

class MythosAction : public GameAction
{
public:
    MythosAction() {
        m_phases = AH::Mythos;
    }
    ~MythosAction() {}

    bool execute();
    virtual QString notificationString(NotificationPart part, const QString &desc = QString::null) const;

    QString id() const { return "AC_MYTHOS"; }
};

class Mythos : public GamePhase
{
public:
    Mythos(Game *game);
    ~Mythos();


protected:
    void enterPhase();
    QList<GameAction *> getPhaseActions();
    QList<GameOption *> getPhaseOptions();
    void finishPhase();

private:
    MythosAction *m_mythos;
    MythosCard *m_activeRumor;
};

#endif // MYTHOS_H
