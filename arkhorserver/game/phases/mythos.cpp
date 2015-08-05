#include "mythos.h"
#include "game/mythoscard.h"
#include "monster.h"

Mythos::Mythos(Game *game)
    : GamePhase(game), m_activeRumor(NULL)
{
    m_mythos = new MythosAction;
}

Mythos::~Mythos()
{
    delete m_mythos;
}

void Mythos::enterPhase()
{
    m_activeRumor = gGame->rumor();
}

QList<GameAction *> Mythos::getPhaseActions()
{
    return QList<GameAction *>()
            << m_mythos;
}

QList<GameOption *> Mythos::getPhaseOptions()
{
    return QList<GameOption *>()
            << getSkipOption();
}

void Mythos::finishPhase()
{
    if (m_activeRumor) {
        m_activeRumor->onMythos();
    }
}

////////////////////////////

bool MythosAction::execute()
{
    MythosCard *card = gGame->drawMythos();

    gGame->notifier()->acknowledgeMythos(card);

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
    switch (card->type()) {
    case AH::Common::MythosData::None:
        gGame->returnMythos(card);
        break;
    case AH::Common::MythosData::Headline:
        card->executeHeadline();
        gGame->returnMythos(card);
        break;
    case AH::Common::MythosData::Environment:
        if (gGame->setEnvironment(card)) {
            GameOption *opt = card->environmentFieldOption();
            if (opt) {
                GameField *f = gGame->board()->field(card->environmentFieldId());
                f->addFieldOption(opt);
                f->setSpecialActionNr(gGame->getSpecialActionNumber());
            }
        } else {
            gGame->returnMythos(card);
        }
        break;
    case AH::Common::MythosData::Rumor:
        if (gGame->setRumor(card)) {
            GameOption *opt = card->rumorFieldOption();
            if (opt) {
                GameField *f = gGame->board()->field(card->rumorFieldId());
                f->addFieldOption(opt);
                f->setSpecialActionNr(gGame->getSpecialActionNumber());
            }
            card->setupRumor();
        } else {
            gGame->returnMythos(card);
        }
        break;
    }

    return true;
}
