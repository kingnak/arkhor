
#include <QApplication>
#include "form.h"
#include "ahmaingui.h"

#include "monsterwidget.h"
#include <monsterdata.h>
using namespace AH::Common;

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Form f;
    f.show();

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

    //AhMainGui gui;
    //gui.show();
    app.exec();
}

