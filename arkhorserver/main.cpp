#ifdef DEBUG_SCRIPT_BUILD
#include <QtWidgets/QApplication>
#else
#include <QCoreApplication>
#endif

#include "game/game.h"
#include "game/gameboard.h"
#include "character.h"
#include "monster.h"
#include "gate.h"
#include "script/gamescript.h"
#include <QScriptEngine>
#include <QFile>
#include <QTextStream>
#include "communication/gameserver.h"
//#include <vld.h>

#include <arkhorscriptgenerator.h>



int main(int argc, char *argv[])
{    
#if 1
#ifdef DEBUG_SCRIPT_BUILD
    QApplication app(argc, argv);
#else
    QCoreApplication app(argc, argv);
#endif

    Game g;
    GameScript *script = new GameScript(&g);

    QString baseDir = "./data";
    if (app.arguments().length() > 1)
        baseDir = app.arguments()[1];

#ifdef _DEBUG
//#error ok
    if (!QDir(baseDir).exists()) {
#ifdef Q_OS_WIN
        baseDir = "D:\\priv\\proj\\ah.git\\arkhorserver\\data\\";
#else
        baseDir = "/home/kingnak/Projects/ah.git/arkhorserver/data/";
#endif

    }
#else
#endif

    if (!script->init(baseDir))
        return 1;

    GameServer srv;
    srv.start();

    app.exec();

    return 0;
#endif

#if 0
    // AHS COMPILER:
    if (argc < 3) {
        return(1);
    }

    QFile fi(argv[1]);
    fi.open(QFile::ReadOnly);
    QFile fo(argv[2]);
    //if (fo.exists()) {
    //    return(2);
    //}
    fo.open(QFile::WriteOnly);
    AHS::ArkhorScriptGenerator p(&fi, &fo);
    if (p.generate()) {
        qDebug() << p.warning();
        return(0);
    }
    qDebug() << p.warning();
    qDebug() << p.error();
    return(3);
#endif


#if 0
    SimpleGameNotifier gn;
    //SimpleGameInteractor gi;
    Game g(&gn, &gn);
    g.init();

    QList<AttributeSlider> sliders;
    {
        AttributeSlider s;
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Speed, 1), AttributeValue(AH::Attr_Sneak, 4)));
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Speed, 2), AttributeValue(AH::Attr_Sneak, 3)));
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Speed, 3), AttributeValue(AH::Attr_Sneak, 2)));
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Speed, 4), AttributeValue(AH::Attr_Sneak, 1)));
        sliders.append(s);
    }

    {
        AttributeSlider s;
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Fight, 1), AttributeValue(AH::Attr_Will, 4)));
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Fight, 2), AttributeValue(AH::Attr_Will, 3)));
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Fight, 3), AttributeValue(AH::Attr_Will, 2)));
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Fight, 4), AttributeValue(AH::Attr_Will, 1)));
        sliders.append(s);
    }

    {
        AttributeSlider s;
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Lore, 1), AttributeValue(AH::Attr_Luck, 4)));
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Lore, 2), AttributeValue(AH::Attr_Luck, 3)));
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Lore, 3), AttributeValue(AH::Attr_Luck, 2)));
        s.addAttributePair(AttributePair(AttributeValue(AH::Attr_Lore, 4), AttributeValue(AH::Attr_Luck, 1)));
        sliders.append(s);
    }

    Player *p = new Player;
    Character *c = new Character(4, 4, 3);
    c->setFocusAttributes(sliders);
    p->setCharacter(c);
    g.board()->field(GameBoard::MD_MerchantDist)->placeCharacter(c);
    g.addPlayer(p);

    p = new Player;
    c = new Character(4, 4, 3);
    c->setFocusAttributes(sliders);
    g.board()->field(GameBoard::RT_Rivertown)->placeCharacter(c);
    p->setCharacter(c);

    Monster *m = new Monster();
    g.board()->field(GameBoard::NS_Northside)->placeMonster(m);

    Gate *pr = new Gate(AH::Dim_Circle, 3, g.board()->field(GameBoard::OW_Abyss_1));
    g.board()->field(GameBoard::NS_Newspaper)->setGate(pr);

    g.addPlayer(p);
    g.play();
#endif
}
