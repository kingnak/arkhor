
#include <QApplication>
#include <QDir>
#include "gui/startform.h"
#include "gui/ahmaingui.h"
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
#include <QResource>
#include "resourcepool.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);

#ifdef _DEBUG
    QDir d = QDir::current();
#else
    QDir d = QDir(QCoreApplication::applicationDirPath());
#endif
    QString rccFile = d.absoluteFilePath("client_resources.rcc");
    if (!QResource::registerResource(rccFile)) {
        qCritical(qPrintable(QString("Cannot open %1").arg(rccFile)));
        return 1;
    }

    QString baseDir = "./res.zip";
    if (app.arguments().length() > 1)
        baseDir = app.arguments()[1];
    //ResourcePool::instance()->addZip("D:/priv/proj/arkhor/arkhor/res.zip");
#ifdef _DEBUG
    if (!QDir(baseDir).exists()) {
#ifdef Q_OS_WIN
        baseDir = "D:/priv/proj/ah.git/res/";
#else
        baseDir = "/home/kingnak/Projects/ah.git/res/";
#endif
    }
#endif
    if (QFileInfo(baseDir).isDir())
        ResourcePool::instance()->addDirectory(baseDir);
    else
        ResourcePool::instance()->addZip(baseDir);

    StartForm f;
    //AhMainGui f;
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

