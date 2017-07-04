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
    connect(ask, &QGroupBox::toggled, [](bool b){ScriptTestConfig::m_doAsk = b;});

    l->addWidget(ask);
    l->addWidget(new QLabel());

    QVBoxLayout *l2 = new QVBoxLayout(ask);
    QHBoxLayout *g = new QHBoxLayout;
    QCheckBox *b = new QCheckBox("Common");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawCommon = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Unique");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawUnique = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Spell");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawSpell = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Ally");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawAlly = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Skill");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawSkill = b;});
    b->setChecked(true);
    g->addWidget(b);

    g->addStretch(1);
    l2->addLayout(g);
    g = new QHBoxLayout;

    b = new QCheckBox("Ancient One");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawAncientOne = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Monster");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawMonster = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Mythos");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawMythos = b;});
    b->setChecked(true);
    g->addWidget(b);
/*
    b = new QCheckBox("Arkham Encounter");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawArkhamEncounter = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Other World Encounter");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawOtherWorldEncounter = b;});
    b->setChecked(true);
    g->addWidget(b);
*/
    g->addStretch(1);
    l2->addLayout(g);

    l->addWidget(ask);
    l->addStretch(1);
}

#endif
