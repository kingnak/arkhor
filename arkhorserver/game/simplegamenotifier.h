#ifndef SIMPLEGAMENOTIFIER_H
#define SIMPLEGAMENOTIFIER_H

#include "gamenotifier.h"
#include "gameinteractor.h"
#include <cost.h>
#include <QTextEdit>
#include <QPair>

class Player;
class QPushButton;
class QEventLoop;

class GUI : public QWidget
{
    Q_OBJECT
public:
    GUI();

public slots:
    void appendText(const QString &txt);
    int chooseOption(const QList<QPair<QString, bool> > &opts);
    int choosePayment(const QList<QPair<AH::Common::CostList, bool> > &costs);

private slots:
    void buttonChosen();

private:
    QWidget *m_buttonContainer;
    QList<QPushButton *> m_buttons;
    QTextEdit *m_txt;
    QEventLoop *m_loop;
    int m_chosen;
};

class SimpleGameNotifier : public GameNotifier, public GameInteractor
{
public:
    SimpleGameNotifier();

    virtual void init(Game *game);

    virtual void nextRound();
    virtual void gamePhaseChaned(AH::GamePhase phase);

    virtual void firstPlayerChanged(const Player *player);
    virtual void currentPlayerChanged(const Player *player);

    virtual void actionStart(const GameAction *action, QString desc = QString::null);
    virtual void actionUpdate(const GameAction *action, QString desc = QString::null);
    virtual void actionFinish(const GameAction *action, QString desc = QString::null);
    virtual void actionExecute(const GameAction *action, QString desc = QString::null);

    virtual Investigator *chooseInvestigator(Player *p, QList<Investigator *> invs);
    virtual GameOption *chooseOption(QList<GameOption *> options);
    virtual QList<int> chooseFocus(QList<AttributeSlider> sliders, int totalFocus);
    virtual MovementPath chooseMovement(GameField *start, int movement);
    virtual AH::Common::CostList choosePayment(const AH::Common::Cost &cost);

private:
    QMap<const Player *, GUI *> m_guis;
};

#endif // SIMPLEGAMENOTIFIER_H
