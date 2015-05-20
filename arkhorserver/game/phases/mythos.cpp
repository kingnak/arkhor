#include "mythos.h"
#include "game/mythoscard.h"
#include "monster.h"

Mythos::Mythos(Game *game)
    : GamePhase(game)
{
    m_mythos = new MythosAction;
}

Mythos::~Mythos()
{
    delete m_mythos;
}

QList<GameAction *> Mythos::getPhaseActions()
{
    return QList<GameAction *>()
            << m_mythos;
}

////////////////////////////

bool MythosAction::execute()
{
    MythosCard *card = gGame->drawMythos();

    // 1. Gate
    GameField *gf = gGame->board()->field(card->gateField());
    gGame->createGate(gf);

    // 2. Clue
    GameField *cf = gGame->board()->field(card->clueField());
    cf->putClue();

    // 3. Monster movement
    AH::Dimensions b = card->blackMovement();
    AH::Dimensions w = card->whiteMovement();
    QList<Monster *> monsters = gGame->board()->getBoardMonsters();
    foreach (Monster *m, monsters) {
        if (b.testFlag(m->dimension())) {
            m->move(AH::MoveBlack);
        } else if (w.testFlag(m->dimension())) {
            m->move(AH::MoveWhite);
        }
    }

    // 4. resolve
    // TODO

    return true;
}
