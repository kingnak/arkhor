#ifdef TEST_SCRIPT_BUILD

#include "scripttestconfigwidget.h"
#include <QtWidgets>
#include "scripttestconfig.h"

ScriptTestConfigWidget::ScriptTestConfigWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinMaxButtonsHint | Qt::WindowStaysOnTopHint);
    QVBoxLayout *l = new QVBoxLayout(this);

    QGroupBox *ask = new QGroupBox("Ask what to draw for:");
    ask->setCheckable(true);
    ask->setChecked(true);
    connect(ask, &QGroupBox::toggled, [](bool b){ScriptTestConfig::doAsk = b;});

    l->addWidget(ask);
    l->addWidget(new QLabel());

    QVBoxLayout *l2 = new QVBoxLayout(ask);
    QHBoxLayout *g = new QHBoxLayout;
    QCheckBox *b = new QCheckBox("Common");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::askDrawCommon = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Unique");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::askDrawUnique = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Spell");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::askDrawSpell = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Ally");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::askDrawAlly = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Skill");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::askDrawSkill = b;});
    b->setChecked(true);
    g->addWidget(b);

    g->addStretch(1);
    l2->addLayout(g);
    g = new QHBoxLayout;

    b = new QCheckBox("Ancient One");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::askDrawAncientOne = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Monster");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::askDrawMonster = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Mythos");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::askDrawMythos = b;});
    b->setChecked(true);
    g->addWidget(b);
/*
    b = new QCheckBox("Arkham Encounter");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::askDrawArkhamEncounter = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Other World Encounter");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::askDrawOtherWorldEncounter = b;});
    b->setChecked(true);
    g->addWidget(b);
*/
    g->addStretch(1);
    l2->addLayout(g);

    l->addWidget(ask);
    l->addStretch(1);
}

#endif
