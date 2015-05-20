#include "simplegamenotifier.h"
#include "game.h"
#include <QtGui>
#include "gameaction.h"
#include "gameoption.h"
#include "player.h"
#include "character.h"

using namespace AH::Common;

SimpleGameNotifier::SimpleGameNotifier()
{
}

void SimpleGameNotifier::init(Game *game)
{
    //m_game = game;
    static bool isInit=false;
    if (isInit) return;
    isInit = true;
    int c = 1;
    foreach (Player *p, game->getPlayers()) {
        GUI *t = new GUI();
        t->setWindowTitle(QString::number(c++));
        t->show();
        m_guis.insert(p, t);
    }
}

void SimpleGameNotifier::nextRound()
{
    foreach (GUI *t, m_guis.values()) {
        t->appendText("\nNext round");
    }
}

void SimpleGameNotifier::gamePhaseChaned(AH::GamePhase phase)
{
    foreach (GUI *t, m_guis.values()) {
        t->appendText("New Phase: ");
        switch (phase) {
        case AH::NoGamePhase: t->appendText("None"); break;
        case AH::Upkeep: t->appendText("Upkeep"); break;
        case AH::Movement: t->appendText("Movement"); break;
        case AH::ArkhamEncountery: t->appendText("Arkham Encountery"); break;
        case AH::OtherWorldEncountery: t->appendText("Other World Encountery"); break;
        case AH::Mythos: t->appendText("Mythos"); break;
        default: t->appendText(QString::number((int)phase)); break;
        }
    }
}

void SimpleGameNotifier::firstPlayerChanged(const Player *player)
{
    m_guis[player]->appendText("You're the first player");
}

void SimpleGameNotifier::currentPlayerChanged(const Player *player)
{
    m_guis[player]->appendText("You're the current player");
}

void SimpleGameNotifier::actionExecute(const GameAction *action, QString desc)
{
    m_guis[gGame->context().player()]->appendText( QString("Execute %1: %2").arg(action->name(), desc) );
}

Investigator *SimpleGameNotifier::chooseInvestigator(Player *p, QList<Investigator *> invs)
{
    Q_UNUSED(p);
    Q_UNUSED(invs);
    //p->
    return NULL;
}

void SimpleGameNotifier::actionStart(const GameAction *action, QString desc)
{
    m_guis[gGame->context().player()]->appendText( QString("Start %1: %2").arg(action->name(), desc) );
}

void SimpleGameNotifier::actionUpdate(const GameAction *action, QString desc)
{
    m_guis[gGame->context().player()]->appendText( QString("Update %1: %2").arg(action->name(), desc) );
}

void SimpleGameNotifier::actionFinish(const GameAction *action, QString desc)
{
    m_guis[gGame->context().player()]->appendText( QString("Finish %1: %2").arg(action->name(), desc) );
}


GameOption *SimpleGameNotifier::chooseOption(QList<GameOption *> options)
{
    QList<QPair<QString, bool> > msg;
    //QList<GameOption *> selectable;
    foreach (GameOption *o, options) {
        //if (o->isAvailable()) {
            msg.append(qMakePair(o->name(), o->isAvailable()));
            //selectable << o;
        //}
    }
    int i = m_guis[gGame->context().player()]->chooseOption(msg);
    //return selectable.value(i);
    return options.value(i);
}

QList<int> SimpleGameNotifier::chooseFocus(QList<AttributeSlider> sliders, int totalFocus)
{
    Q_UNUSED(totalFocus);

    QDialog dlg;
    QVBoxLayout *v = new QVBoxLayout(&dlg);

    QButtonGroup *grpSS = new QButtonGroup;
    QHBoxLayout *l = new QHBoxLayout;
    foreach (AttributePair p, sliders[0].availableSettings()) {
        QRadioButton *rb = new QRadioButton(QString("Speed: %1, Sneak %2").arg(p.first().value).arg(p.second().value));
        grpSS->addButton(rb);
        l->addWidget(rb);
        if (sliders[0].currentSetting() == p) {
            rb->setChecked(true);
        }
    }
    v->addLayout(l);


    QButtonGroup *grpFW = new QButtonGroup;
    l = new QHBoxLayout;
    foreach (AttributePair p, sliders[1].availableSettings()) {
        QRadioButton *rb = new QRadioButton(QString("Fight: %1, Will %2").arg(p.first().value).arg(p.second().value));
        grpFW->addButton(rb);
        l->addWidget(rb);
        if (sliders[1].currentSetting() == p) {
            rb->setChecked(true);
        }
    }
    v->addLayout(l);

    QButtonGroup *grpLL = new QButtonGroup;
    l = new QHBoxLayout;
    foreach (AttributePair p, sliders[2].availableSettings()) {
        QRadioButton *rb = new QRadioButton(QString("Lore: %1, Luck %2").arg(p.first().value).arg(p.second().value));
        grpLL->addButton(rb);
        l->addWidget(rb);
        if (sliders[2].currentSetting() == p) {
            rb->setChecked(true);
        }
    }
    v->addLayout(l);

    QPushButton *ok = new QPushButton("ok");
    v->addWidget(ok);
    QObject::connect(ok, SIGNAL(clicked()), &dlg, SLOT(accept()));
    dlg.exec();

    QList<int> ret;
    int idx = grpSS->buttons().indexOf(grpSS->checkedButton());
    int diff = sliders[0].distanceBetween(sliders[0].currentSetting(), sliders[0].availableSettings()[idx]);
    ret << diff;

    idx = grpFW->buttons().indexOf(grpFW->checkedButton());
    diff = sliders[1].distanceBetween(sliders[1].currentSetting(), sliders[1].availableSettings()[idx]);
    ret << diff;

    idx = grpLL->buttons().indexOf(grpLL->checkedButton());
    diff = sliders[2].distanceBetween(sliders[2].currentSetting(), sliders[2].availableSettings()[idx]);
    ret << diff;

    return ret;
}

MovementPath SimpleGameNotifier::chooseMovement(GameField *start, int movement)
{
    MovementPath p;
    do {
        p << start;
        QMessageBox msg;
        QList<GameField*> fields;
        foreach (GameField *f, start->neighbours()) {
            msg.addButton(f->name(), QMessageBox::ActionRole);
            fields << f;
        }
        msg.addButton("Stop", QMessageBox::ActionRole);
        msg.exec();
        start = fields.value(msg.buttons().indexOf(msg.clickedButton()));
        //if (start) p << start;
    } while (start && movement-- > 0);

    return p;
}

CostList SimpleGameNotifier::choosePayment(const Cost &cost)
{
    Player *p = gGame->context().player();
    QList<QPair<CostList, bool> > cl;
    foreach (CostList l, cost.getAlternatives()) {
        cl.append(qMakePair(l, p->getCharacter()->canPay(l)));
    }

    int idx = m_guis[p]->choosePayment(cl);

    if (idx >= 0) {
        return cl[idx].first;
    }

    return CostList::invalid();
}

///////////////////////

GUI::GUI()
{
    m_buttonContainer = new QWidget(this);
    m_txt = new QTextEdit(this);
    m_txt->setReadOnly(true);
    m_txt->setAcceptRichText(false);
    QBoxLayout *l = new QVBoxLayout(this);
    l->addWidget(m_txt);
    l->addWidget(m_buttonContainer);
    m_buttonContainer->setLayout(new QHBoxLayout);
    m_buttonContainer->setVisible(false);
    m_loop = new QEventLoop;
}

void GUI::appendText(const QString &txt)
{
    m_txt->append(txt);
    m_txt->moveCursor(QTextCursor::End);
    m_txt->ensureCursorVisible();
}

int GUI::chooseOption(const QList<QPair<QString, bool> > &opts)
{
    typedef QPair<QString, bool> LocalForEachType;
    foreach (const LocalForEachType &s, opts) {
        QPushButton *b = new QPushButton(s.first, m_buttonContainer);
        m_buttonContainer->layout()->addWidget(b);
        connect(b, SIGNAL(clicked()), this, SLOT(buttonChosen()));
        m_buttons.append(b);
        if (!s.second) b->setEnabled(false);
    }

    m_buttonContainer->setVisible(true);
    m_buttonContainer->update();
    m_chosen = -1;
    m_loop->exec();
    m_buttonContainer->setVisible(false);
    m_buttons.clear();

    QLayoutItem *child;
    while ((child = m_buttonContainer->layout()->takeAt(0)) != 0) {
        delete child->widget();
        delete child;
    }

    return m_chosen;
}

int GUI::choosePayment(const QList<QPair<CostList, bool> > &costs)
{
    QList<QPair<QString, bool> > opts;
    typedef QPair<CostList, bool> LocalForEachType;
    foreach (LocalForEachType l, costs) {
        QStringList line;
        foreach (CostItem i, l.first) {
            QString s;
            switch (i.type) {
            case CostItem::Pay_Money: s = "%1 $"; break;
            case CostItem::Pay_Clue: s = "%1 clues"; break;
            case CostItem::Pay_Stamina: s = "%1 stamina"; break;
            case CostItem::Pay_Sanity: s = "%1 sanity"; break;
            case CostItem::Pay_Movement: s = "%1 movement points"; break;
            case CostItem::Pay_GateTrophy: s = "%1 gate trophies"; break;
            case CostItem::Pay_MonsterTrophy: s = "%1 monster trophies"; break;
            case CostItem::Pay_MonsterToughness: s = "monster trophies for %1 stamina"; break;
            default: s = "%1 ????";
            }
            line << s.arg(i.amount);
        }
        opts << qMakePair(line.join(" & "), l.second);
    }

    opts.prepend(qMakePair(QString("Cancel"), true));
    return chooseOption(opts)-1;
}

void GUI::buttonChosen()
{
    m_chosen = m_buttons.indexOf(qobject_cast<QPushButton*> (sender()));
    m_loop->quit();
}
