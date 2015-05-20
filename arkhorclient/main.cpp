
#include <QApplication>
#include "form.h"
#include "ahmaingui.h"
/*
#include "monsterwidget.h"
#include <monsterdata.h>
using namespace AH::Common;
*/
/*
#include <QStyle>
#include "flowlayout.h"
#include "diewidget.h"
*/
/*
#include "gatewidget.h"
using namespace AH::Common;
*/
#include "resourcepool.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

    //ResourcePool::instance()->addZip("D:/priv/proj/arkhor/arkhor/res.zip");
    ResourcePool::instance()->addDirectory("D:/priv/proj/arkhor/arkhor/res/");

    Form f;
    f.show();

    /*
    QWidget w;
    FlowLayout *l = new FlowLayout(&w);
    DieWidget *d = new DieWidget;
    d->setDieValue(0);
    l->addWidget(d);
    w.show();
    */

    /*
    MonsterData m;
    m.m_name = "TestMonster";
    m.m_awareness = -2;
    m.m_movement = MonsterData::Flying;
    m.m_dimension = AH::Dim_Square;
    m.m_toughness = 1;
    m.m_combatAdjustment = -2;
    m.m_combatDamage = 2;
    m.m_horrorAdjustment = -1;
    m.m_horrorDamage = 2;
    m.m_attributes = MonsterData::Endless;
    m.m_attributes |= MonsterData::Nightmarish_2;

    MonsterWidget w;
    w.showBoth();
    w.displayMonster(&m);
    w.show();
    */
    /*
    GateData g;
    g.m_open = true;
    g.m_adjustment = -3;
    g.m_dest = FieldData::OW_Yuggoth;
    g.m_dims = AH::Dim_Triangle;
    GateWidget w;
    w.displayGate(&g);
    w.show();
    */
    //AhMainGui gui;
    //gui.show();
    app.exec();
}

