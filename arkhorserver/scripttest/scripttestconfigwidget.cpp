#ifdef TEST_SCRIPT_BUILD

#include "scripttestconfigwidget.h"
#include <QtWidgets>
#include "scripttestconfig.h"

ScriptTestConfigWidget::ScriptTestConfigWidget(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint | Qt::WindowStaysOnTopHint);
    QVBoxLayout *l = new QVBoxLayout(this);

    l->addWidget(buildAsk());
    l->addWidget(buildDebug());

    QHBoxLayout *h = new QHBoxLayout;
    QCheckBox *b = new QCheckBox("Die rolls");
    b->setChecked(true);
    connect(b, &QCheckBox::toggled, [](bool b) {ScriptTestConfig::m_askRollDie = b;});
    h->addWidget(b);
    l->addLayout(h);

    l->addStretch(1);
}

QWidget *ScriptTestConfigWidget::buildAsk()
{
    QGroupBox *ask = new QGroupBox("Ask what to draw for:");
    ask->setCheckable(true);
    ask->setChecked(true);
    connect(ask, &QGroupBox::toggled, [](bool b){ScriptTestConfig::m_doAsk = b;});

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

    b = new QCheckBox("Arkham Encounter");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawArkhamEncounter = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Other World Encounter");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_askDrawOtherWorldEncounter = b;});
    b->setChecked(true);
    g->addWidget(b);

    g->addStretch(1);
    l2->addLayout(g);

    return ask;
}

QWidget *ScriptTestConfigWidget::buildDebug()
{
    QGroupBox *dbg = new QGroupBox("Debug");
    dbg->setCheckable(true);
    dbg->setChecked(true);
    connect(dbg, &QGroupBox::toggled, [](bool b){ScriptTestConfig::m_debugScript = b;});

    QVBoxLayout *l2 = new QVBoxLayout(dbg);
    QHBoxLayout *g = new QHBoxLayout;
    QCheckBox *b = new QCheckBox("Action");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_debugAction = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Available");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_debugAvailable = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Prevent");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_debugPrevent = b;});
    b->setChecked(true);
    g->addWidget(b);

    g->addStretch(1);
    l2->addLayout(g);
    g = new QHBoxLayout;

    b = new QCheckBox("Monster");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_debugMonster = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Rumor");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_debugRumor = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("AncientOne");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_debugAncientOne = b;});
    b->setChecked(true);
    g->addWidget(b);

    g->addStretch(1);
    l2->addLayout(g);
    g = new QHBoxLayout;

    b = new QCheckBox("Display");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_debugDisplay = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Modification");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_debugModification = b;});
    b->setChecked(true);
    g->addWidget(b);

    b = new QCheckBox("Inventory");
    connect(b, &QCheckBox::toggled, [](bool b){ScriptTestConfig::m_debugInventory = b;});
    b->setChecked(true);
    g->addWidget(b);

    g->addStretch(1);
    l2->addLayout(g);

    return dbg;
}

#endif
