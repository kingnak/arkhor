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
    ~MythosAction() override = default;

    bool execute() override;
    QString notificationString(NotificationPart part, const QString &desc = QString::null) const override;

    QString id() const override { return "AC_MYTHOS"; }
};

class Mythos : public GamePhase
{
public:
    Mythos(Game *game);
    ~Mythos() override;


protected:
    void enterPhase() override;
    QList<GameAction *> getPhaseActions() override;
    QList<GameOption *> getPhaseOptions() override;
    void finishPhase() override;

private:
    MythosAction *m_mythos;
    MythosCard *m_activeRumor;
};

#endif // MYTHOS_H
