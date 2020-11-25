#include "mythos.h"
#include "game/mythoscard.h"
#include "monster.h"
#include "ancientone.h"

Mythos::Mythos(Game *game)
    : GamePhase(game)
    , m_activeRumor(nullptr)
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
    if (gGame->ancientOne()) {
        gGame->ancientOne()->onEndMythos();
    }
}

////////////////////////////

bool MythosAction::execute()
{
    MythosCard *card = gGame->drawMythos();

    gGame->notifier()->actionStart(this, card->name());

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
    for (auto m : monsters) {
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
    {
            bool activate = true;
            if (gGame->ancientOne()->ignoreEnvironmentTypes().contains(card->environmenType())) {
                activate = false;
            } else {
                activate = gGame->setEnvironment(card);
            }

            if (activate) {
                auto fId = card->environmentFieldId();
                if (fId != AH::Common::FieldData::NO_NO_FIELD) {
                    GameField *f = gGame->board()->field(fId);
                    GameOption *opt = card->environmentFieldOption();
                    f->setSpecialActionNr(gGame->getSpecialActionNumber());
                    if (opt) {
                        f->addFieldOption(opt);
                    }
                }
                card->activateEnvironment();
            } else {
                gGame->returnMythos(card);
            }
        break;
    }
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

    gGame->notifier()->actionFinish(this, card->name());

    return true;
}

QString MythosAction::notificationString(GameAction::NotificationPart part, const QString &desc) const
{
    Q_UNUSED(desc);
    if (part == Start) {
        return "Mythos {D}";
    }
    return QString::null;
}
