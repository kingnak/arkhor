#ifdef TEST_SCRIPT_BUILD

#include <ahglobal.h>
#include "scripttestdrawwidget.h"
#include <QtWidgets>
#include <fielddata.h>
#include "../game/game.h"
#include "../game/gameboard.h"

QString string4dim(AH::Dimension d)
{
	switch (d) {
	case AH::NoDimension: return "No Dimension";
	case AH::Dim_Square: return "Square";
	case AH::Dim_Circle: return "Circle";
	case AH::Dim_Moon: return "Moon";
	case AH::Dim_Slash: return "Slash";
	case AH::Dim_Star: return "Star";
	case AH::Dim_Diamond: return "Diamond";
	case AH::Dim_Hexagon: return "Hexagon";
	case AH::Dim_Plus: return "Plus";
	case AH::Dim_Triangle: return "Triangle";
	}
    return "??? " + QString::number(int(d));
}

void forDimensions(std::function<void(AH::Dimension,QString)> cb, bool withNone = true)
{
	if (withNone) {
		cb(AH::NoDimension, string4dim(AH::NoDimension));
	}
	for (int i = AH::Dim_Square; i <= AH::Dim_Triangle; i<<=1) {
		cb(static_cast<AH::Dimension>(i), string4dim(static_cast<AH::Dimension>(i)));
	}
}

QString string4field(AH::Common::FieldData::FieldID f) {
    switch (f) {
    case AH::Common::FieldData::NO_NO_FIELD: return "No Field";
    case AH::Common::FieldData::NS_Northside: return "NS_Northside";
    case AH::Common::FieldData::NS_TrainStation: return "NS_TrainStation";
    case AH::Common::FieldData::NS_Newspaper: return "NS_Newspaper";
    case AH::Common::FieldData::NS_CuriositieShoppe: return "NS_CuriositieShoppe";
    case AH::Common::FieldData::DT_Downtown: return "DT_Downtown";
    case AH::Common::FieldData::DT_BankOfArhham: return "DT_BankOfArhham";
    case AH::Common::FieldData::DT_ArkhamAsylum: return "DT_ArkhamAsylum";
    case AH::Common::FieldData::DT_IndependenceSquare: return "DT_IndependenceSquare";
    case AH::Common::FieldData::ET_Easttown: return "ET_Easttown";
    case AH::Common::FieldData::ET_HibbsRoadhouse: return "ET_HibbsRoadhouse";
    case AH::Common::FieldData::ET_VelmasDiner: return "ET_VelmasDiner";
    case AH::Common::FieldData::ET_PoliceStation: return "ET_PoliceStation";
    case AH::Common::FieldData::MD_MerchantDist: return "MD_MerchantDist";
    case AH::Common::FieldData::MD_UnvisitedIsle: return "MD_UnvisitedIsle";
    case AH::Common::FieldData::MD_RiverDocks: return "MD_RiverDocks";
    case AH::Common::FieldData::MD_TheUnnamable: return "MD_TheUnnamable";
    case AH::Common::FieldData::RT_Rivertown: return "RT_Rivertown";
    case AH::Common::FieldData::RT_Graveyard: return "RT_Graveyard";
    case AH::Common::FieldData::RT_BlackCave: return "RT_BlackCave";
    case AH::Common::FieldData::RT_GeneralStore: return "RT_GeneralStore";
    case AH::Common::FieldData::MU_MiskatonicU: return "MU_MiskatonicU";
    case AH::Common::FieldData::MU_ScienceBuilding: return "MU_ScienceBuilding";
    case AH::Common::FieldData::MU_Administration: return "MU_Administration";
    case AH::Common::FieldData::MU_Library: return "MU_Library";
    case AH::Common::FieldData::FH_FrenchHill: return "FH_FrenchHill";
    case AH::Common::FieldData::FH_TheWitchHouse: return "FH_TheWitchHouse";
    case AH::Common::FieldData::FH_SilverTwilightLodge: return "FH_SilverTwilightLodge";
    case AH::Common::FieldData::UT_Uptown: return "UT_Uptown";
    case AH::Common::FieldData::UT_StMarysHospital: return "UT_StMarysHospital";
    case AH::Common::FieldData::UT_YeOldeMagickShoppe: return "UT_YeOldeMagickShoppe";
    case AH::Common::FieldData::UT_Woods: return "UT_Woods";
    case AH::Common::FieldData::SS_Southside: return "SS_Southside";
    case AH::Common::FieldData::SS_MasBoardingHouse: return "SS_MasBoardingHouse";
    case AH::Common::FieldData::SS_SouthChurch: return "SS_SouthChurch";
    case AH::Common::FieldData::SS_HistoricalSociety: return "SS_HistoricalSociety";
    case AH::Common::FieldData::OW_AnotherDimension: return "OW_AnotherDimension";
    case AH::Common::FieldData::OW_Abyss: return "OW_Abyss";
    case AH::Common::FieldData::OW_CityOfGreatRace: return "OW_CityOfGreatRace";
    case AH::Common::FieldData::OW_Yuggoth: return "OW_Yuggoth";
    case AH::Common::FieldData::OW_Celeano: return "OW_Celeano";
    case AH::Common::FieldData::OW_TheDreamlands: return "OW_TheDreamlands";
    case AH::Common::FieldData::OW_PlateauOfLeng: return "OW_PlateauOfLeng";
    case AH::Common::FieldData::OW_R_lyeh: return "OW_R_lyeh";
    case AH::Common::FieldData::Sp_Sky: return "Sp_Sky";
    case AH::Common::FieldData::Sp_Outskirts: return "Sp_Outskirts";
    case AH::Common::FieldData::Sp_SpaceAndTime: return "Sp_SpaceAndTime";
    }
    return "??? " + QString::number(int(f));
}

void forFields(std::function<void(AH::Common::FieldData::FieldID,QString)> cb, bool withNone = true)
{
    if (withNone) {
        cb(AH::Common::FieldData::NO_NO_FIELD, string4field(AH::Common::FieldData::NO_NO_FIELD));
    }
    // Streets + locations
    for (int r = 0x0100; r <= 0x0900; r += 0x0100) {
        for (int c = 0; c < 4; ++c) {
            auto f = static_cast<AH::Common::FieldData::FieldID> (r+c);
            if (r == AH::Common::FieldData::FH_FrenchHill && c == 3)
                continue; // French hill has only 3 fields
            cb(f, string4field(f));
        }
    }
    // Other world
    for (int o = 0x1000; o <= 0x1700; o += 0x0100) {
        auto f = static_cast<AH::Common::FieldData::FieldID> (o);
        cb(f, string4field(f));
    }
    // Special
    for (int s = 0x2000; s <= 0x2002; ++s) {
        auto f = static_cast<AH::Common::FieldData::FieldID> (s);
        cb(f, string4field(f));
    }
}

QComboBox *setupCmbForFields(QList<AH::Common::FieldData::FieldType> types)
{
    QComboBox *ret = new QComboBox;
    ret->setEditable(true);
    ret->setInsertPolicy(QComboBox::NoInsert);
    QStringList itms;
    forFields([=,&itms](AH::Common::FieldData::FieldID f, QString s) {
        auto fld = gGame->board()->field(f);
        auto t = fld ? fld->type() : static_cast<AH::Common::FieldData::FieldType> (0);
        if (f == AH::Common::FieldData::NO_NO_FIELD || types.contains(t)) {
            ret->addItem(s, QVariant(int(f)));
            itms.append(s);
        }
    }, true);
    QCompleter *c = new QCompleter(itms);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    c->setFilterMode(Qt::MatchContains);
    ret->setCompleter(c);
    return ret;
}

////////////////////////////////////////

ScriptTestDrawWidget::ScriptTestDrawWidget(QWidget *parent) : QDialog(parent)
{
	setupUi(new QHBoxLayout(this));
}

ScriptTestDrawWidget::ScriptTestDrawWidget(QWidget *parent, bool) : QDialog(parent), m_cmb(nullptr), m_title(nullptr), m_rnd(nullptr)
{
}

void ScriptTestDrawWidget::setupUi(QHBoxLayout *l)
{
	m_title = new QLabel;
    l->addWidget(m_title);

    m_cmb = new QComboBox;
    m_cmb->setEditable(true);
    m_cmb->setInsertPolicy(QComboBox::NoInsert);
    l->addWidget(m_cmb, 1);

    setupButtons(l);
}

void ScriptTestDrawWidget::setupButtons(QHBoxLayout *l)
{
    QPushButton *b = new QPushButton("OK");
    b->setDefault(true);
    b->setAutoDefault(true);
    connect(b, &QPushButton::clicked, this, &QDialog::accept);
    l->addWidget(b);
    b = new QPushButton("Random");
    connect(b, &QPushButton::clicked, this, &QDialog::reject);
	l->addWidget(b);
}

QString ScriptTestDrawWidget::askDraw(const QString &title, QStringList lst)
{
	prepareShow(title, lst);
	return doShow();
}

void ScriptTestDrawWidget::prepareShow(const QString &title, QStringList lst)
{
    m_title->setText("Draw " + title);
    qSort(lst);
    m_cmb->clear();
    m_cmb->addItems(lst);

    QCompleter *c = new QCompleter(lst, this);
    c->setCaseSensitivity(Qt::CaseInsensitive);
	c->setFilterMode(Qt::MatchContains);
    m_cmb->setCompleter(c);

	m_cmb->lineEdit()->selectAll();
}

QString ScriptTestDrawWidget::doShow()
{
    show();
    activateWindow();
    raise();

    // Run a separate event loop so other debug windows respond (non-modal, blocking)
    QEventLoop l;
    connect(this, &QDialog::accepted, &l, &QEventLoop::quit);
    connect(this, &QDialog::rejected, &l, &QEventLoop::quit);
    l.exec();

    if (result() == QDialog::Accepted) {
        return m_cmb->currentText();
    }
    return QString::null;
}

/////////////////////////////////////

ScriptTestDrawMonsterWidget::ScriptTestDrawMonsterWidget(QWidget *parent)
	: ScriptTestDrawWidget(parent, true)
{
	QHBoxLayout *l = new QHBoxLayout;
	ScriptTestDrawWidget::setupUi(l);

	QVBoxLayout *v = new QVBoxLayout(this);
	v->addLayout(l);

	l = new QHBoxLayout;
	m_dim = new QComboBox;
	forDimensions([=](AH::Dimension d, QString s){
		m_dim->addItem(s, QVariant(int(d)));
	});

	l->addWidget(new QLabel("Dimension: "));
	l->addWidget(m_dim, 1);

	v->addLayout(l);
	v->addStretch(1);
}

QString ScriptTestDrawMonsterWidget::askDraw(const QString&, QStringList monsters)
{
	return ScriptTestDrawWidget::askDraw("Monster", monsters);
}

QString ScriptTestDrawMonsterWidget::moreData()
{
	int i = m_dim->currentData().toInt();
	return QString::number(i);
}

/////////////////////////////////////

ScriptTestDrawMythosWidget::ScriptTestDrawMythosWidget(QWidget *parent)
    : ScriptTestDrawWidget(parent, true),
      m_black(0),
      m_white(0)
{
    QHBoxLayout *l = new QHBoxLayout;
    ScriptTestDrawWidget::setupUi(l);

    QVBoxLayout *v = new QVBoxLayout(this);
    v->addLayout(l);

    m_clue = setupCmbForFields(QList<AH::Common::FieldData::FieldType>() << AH::Common::FieldData::Location);
    l = new QHBoxLayout;
    l->addWidget(new QLabel("Clue: "));
    l->addWidget(m_clue, 1);
    v->addLayout(l);

    // TODO: Only stable?
    m_gate = setupCmbForFields(QList<AH::Common::FieldData::FieldType>() << AH::Common::FieldData::Location);
    l = new QHBoxLayout;
    l->addWidget(new QLabel("Gate: "));
    l->addWidget(m_gate, 1);
    v->addLayout(l);


    QGroupBox *black = new QGroupBox("Black movement");
    QGroupBox *white = new QGroupBox("White movement");
    QGridLayout *g1 = new QGridLayout(black);
    QGridLayout *g2 = new QGridLayout(white);
    int r = 0, c = 0;
    forDimensions([=,&r,&c](auto t, auto s){
        QCheckBox *c1 = new QCheckBox(s);
        QCheckBox *c2 = new QCheckBox(s);

        connect(c1, &QCheckBox::toggled, [=](bool b) {
            if (b) { c2->setChecked(false); m_white &= ~t; }
            if (b) m_black |= t; else m_black &= ~t;
        });
        connect(c2, &QCheckBox::toggled, [=](bool b) {
            if (b) { c1->setChecked(false); m_black &= ~t; }
            if (b) m_white |= t; else m_white &= ~t;
        });
        g1->addWidget(c1, r, c);
        g2->addWidget(c2, r, c++);
        if (c == 5) {
            c = 0;
            r++;
        }
    }, false);

    v->addWidget(white);
    v->addWidget(black);

    v->addStretch(1);
}

QString ScriptTestDrawMythosWidget::askDraw(const QString&, QStringList monsters)
{
    m_white = m_black = 0;
    return ScriptTestDrawWidget::askDraw("Mythos", monsters);
}

QString ScriptTestDrawMythosWidget::moreData()
{
    QStringList lst = QStringList()
            << QString::number(m_gate->currentData().toInt())
            << QString::number(m_clue->currentData().toInt())
            << QString::number(m_white)
            << QString::number(m_black);
    return lst.join(';');
}

/////////////////////////////////////

ScriptTestDrawGateWidget::ScriptTestDrawGateWidget(QWidget *parent)
    : ScriptTestDrawWidget(parent, false), m_dim(0), m_adj(0)
{
    QSpinBox *spn = new QSpinBox;
    spn->setRange(-2, 2);
    spn->setValue(0);
    connect(spn, (void(QSpinBox::*)(int))&QSpinBox::valueChanged, [=](int v) {m_adj = v;});

    QVBoxLayout *v = new QVBoxLayout(this);
    QHBoxLayout *l = new QHBoxLayout;
    m_title = new QLabel("Gate");
    l->addWidget(m_title);
    m_cmb = setupCmbForFields(QList<AH::Common::FieldData::FieldType>() << AH::Common::FieldData::OtherWorld);
    l->addWidget(m_cmb, 1);
    l->addWidget(spn);
    setupButtons(l);
    v->addLayout(l);

    QGroupBox *dim = new QGroupBox("Dimensions");
    QGridLayout *g1 = new QGridLayout(dim);
    int r = 0, c = 0;
    forDimensions([=,&r,&c](auto t, auto s){
        QCheckBox *c1 = new QCheckBox(s);

        connect(c1, &QCheckBox::toggled, [=](bool b) {
            if (b) m_dim |= t; else m_dim &= ~t;
        });

        g1->addWidget(c1, r, c++);
        if (c == 5) {
            c = 0;
            r++;
        }
    }, false);

    v->addWidget(dim);
}

QString ScriptTestDrawGateWidget::askDraw(const QString &, QStringList)
{
    connect(this, &QDialog::rejected, [=](){m_adj=-10;});
    ScriptTestDrawWidget::doShow();
    if (m_adj == -10) return QString::null;
    return moreData();
}

QString ScriptTestDrawGateWidget::moreData()
{
    return (QStringList()
            << QString::number(m_cmb->currentData().toInt())
            << QString::number(m_adj)
            << QString::number(m_dim)
            ).join(';');
}

#endif
