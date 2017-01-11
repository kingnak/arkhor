#include "optionchooser.h"
#include "ui_optionchooser.h"
#include <QtWidgets>
//#include "flowlayout.h"
#include "utils.h"
#include "objectregistry.h"
#include "monsterwidget.h"
#include "gameobjectwidget.h"
#include "gatedatawidget.h"

using namespace AH::Common;

static const char *OPTION_DESCRIPTION_PROPERTY = "DESCRIPTION";
static const char *OPTION_COST_PROPERTY = "COSTS";
static const char *OPTION_PROPERTY = "OPTION";
static const char *OPTION_ID_PROPERTY = "ID";
static const char *OPTION_SKILL_PROPERTY = "SKILL";
static const char *OPTION_MONSTER_PROPERTY = "MONSTER";
static const char *OPTION_OBJECT_PROPERTY = "OBJECT";
static const char *OPTION_GATE_PROPERTY = "GATE";

OptionChooser::OptionChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionChooser),
    m_moreWgt(NULL)
{
    ui->setupUi(this);
    //ui->wgtOptionsList->setLayout(new FlowLayout);
    ui->wgtOptionsList->setLayout(new QVBoxLayout);
}

OptionChooser::~OptionChooser()
{
    delete ui;
}

void OptionChooser::setOptions(QList<AH::Common::GameOptionData> opts)
{
    m_type = ChooseOption;
    cleanupOptions();

    QSet<QChar> usedMnemonics;
    usedMnemonics.insert('o');

    QLayout *l = ui->wgtOptionsList->layout();
    foreach (GameOptionData o, opts) {
        QString name = o.name();
        for (int i = 0; i < name.length(); ++i) {
            QChar c = name.at(i).toLower();
            if (!usedMnemonics.contains(c)) {
                usedMnemonics.insert(c);
                name.insert(i, '&');
                break;
            }
        }

        QPushButton *b = new QPushButton(name);
        b->setCheckable(true);
        b->setProperty(OPTION_DESCRIPTION_PROPERTY, o.description());
        b->setProperty(OPTION_COST_PROPERTY, displayCosts(o.costs()));
        b->setProperty(OPTION_ID_PROPERTY, o.id());
        if (o.description().isEmpty() && !o.sourceId().isEmpty()) {
            ObjectRegistry::instance()->asyncGetObject(new OptionDescUpdater(b), o.sourceId());
        }
        QVariant v;
        v << o;
        b->setProperty(OPTION_PROPERTY, v);
        connect(b, SIGNAL(clicked()), this, SLOT(showOption()));
        l->addWidget(b);
    }

    ui->btnOptionActivate->setDefault(true);

    //ui->wgtOptionsList->setLayout(l);
}

void OptionChooser::setSkills(QList<ModifiedPropertyValueData> opts)
{
    m_type = ChooseSkill;
    cleanupOptions();

    QLayout *l = ui->wgtOptionsList->layout();
    foreach (ModifiedPropertyValueData v, opts) {
        QString name = Utils::stringForProperty(v.property().property());
        int val = v.finalVal();
        QPushButton *btn = new QPushButton(name);
        btn->setCheckable(true);
        btn->setProperty(OPTION_DESCRIPTION_PROPERTY, QString("Skill %1. Current Value: %2").arg(name).arg(val));
        btn->setProperty(OPTION_SKILL_PROPERTY, QVariant::fromValue(static_cast<qint32>(v.property().property())));
        connect(btn, SIGNAL(clicked()), this, SLOT(showOption()));
        l->addWidget(btn);
    }
}

void OptionChooser::setEncounter(EncounterData enc)
{
    m_type = ChooseEncounter;
    QString baseDesc = enc.description();

    if (enc.optionData().size() > 1)
    {
        QSet<QChar> usedMnemonics;
        usedMnemonics.insert('o');

        QLayout *l = ui->wgtOptionsList->layout();
        foreach (GameOptionData o, enc.optionData()) {
            QString name = o.name();
            for (int i = 0; i < name.length(); ++i) {
                QChar c = name.at(i).toLower();
                if (!usedMnemonics.contains(c)) {
                    usedMnemonics.insert(c);
                    name.insert(i, '&');
                    break;
                }
            }

            QPushButton *b = new QPushButton(name);
            b->setCheckable(true);
            b->setProperty(OPTION_DESCRIPTION_PROPERTY, baseDesc + "\n\n" + o.description());
            b->setProperty(OPTION_COST_PROPERTY, displayCosts(o.costs()));
            b->setProperty(OPTION_ID_PROPERTY, o.id());
            QVariant v;
            v << o;
            b->setProperty(OPTION_PROPERTY, v);
            connect(b, SIGNAL(clicked()), this, SLOT(showOption()));
            l->addWidget(b);
        }
    } else {
        QString id = enc.optionData().value(0).id();
        ui->btnOptionActivate->setProperty(OPTION_ID_PROPERTY, id);
        ui->btnOptionActivate->setEnabled(true);
    }

    ui->lblOptionDescription->setText(baseDesc);

    ui->btnOptionActivate->setDefault(true);
}

void OptionChooser::cleanupOptions()
{
    QLayout *l = ui->wgtOptionsList->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
    }
    foreach (QWidget *w, ui->wgtOptionsList->findChildren<QWidget*>()) {
        delete w;
    }

    ui->lblOptionDescription->setText("");
    ui->btnOptionActivate->setEnabled(false);

    cleanupMore();
}

void OptionChooser::cleanupMore()
{
    /*
    QLayout *l = ui->wgtMoreDescription->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            if (child->widget()) delete child->widget();
            delete child;
        }
    }
    */
    /*
    foreach (QWidget *w, ui->wgtMoreDescription->findChildren<QWidget*>()) {
        delete w;
    }
    */
    delete m_moreWgt;
    m_moreWgt = NULL;
}

void OptionChooser::showOption()
{
    QList<QPushButton*> btns = ui->wgtOptionsList->findChildren<QPushButton*>();
    foreach (QPushButton *b, btns) {
        b->setChecked(false);
    }
    qobject_cast<QPushButton*> (sender())->setChecked(true);

    QString s = sender()->property(OPTION_DESCRIPTION_PROPERTY).toString() + sender()->property(OPTION_COST_PROPERTY).toString();
    ui->lblOptionDescription->setText(s);
    ui->btnOptionActivate->setProperty(OPTION_ID_PROPERTY, sender()->property(OPTION_ID_PROPERTY));
    ui->btnOptionActivate->setProperty(OPTION_SKILL_PROPERTY, sender()->property(OPTION_SKILL_PROPERTY));

    ui->btnOptionActivate->setEnabled(true);

    QVariant v = sender()->property(OPTION_PROPERTY);
    if (v.isValid()) {
        GameOptionData o;
        v >> o;
        if (!o.isAvailable() || !o.canPay()) {
            ui->btnOptionActivate->setEnabled(false);
        }
    }

    cleanupMore();

    v = sender()->property(OPTION_MONSTER_PROPERTY);
    if (v.isValid() && !v.toString().isEmpty()) {
        DescribeObjectsData::ObjectDescription desc = ObjectRegistry::instance()->getObject(v.toString());
        if (desc.type != RequestObjectsData::Unknown) {
            MonsterWidget *w = new MonsterWidget(this);
            MonsterData m;
            desc.data >> m;
            w->displayMonster(&m);
            setMoreWidget(w);
        }
        return;
    }

    v = sender()->property(OPTION_OBJECT_PROPERTY);
    if (v.isValid() && !v.toString().isEmpty()) {
        DescribeObjectsData::ObjectDescription desc = ObjectRegistry::instance()->getObject(v.toString());
        if (desc.type != RequestObjectsData::Unknown) {
            GameObjectWidget *w = new GameObjectWidget(this);
            GameObjectData o;
            desc.data >> o;
            w->displayGameObject(&o);
            setMoreWidget(w);
        }
        return;
    }

    v = sender()->property(OPTION_GATE_PROPERTY);
    if (v.isValid() && !v.toString().isEmpty()) {
        DescribeObjectsData::ObjectDescription desc = ObjectRegistry::instance()->getObject(v.toString());
        if (desc.type != RequestObjectsData::Unknown) {
            GateDataWidget *w = new GateDataWidget(this);
            GateData g;
            desc.data >> g;
            w->displayGate(&g);
            setMoreWidget(w);
        }
        return;
    }
}

void OptionChooser::on_btnOptionActivate_clicked()
{
    cleanupOptions();

    if (m_type == ChooseOption) {
        QString id = ui->btnOptionActivate->property(OPTION_ID_PROPERTY).toString();
        emit optionChosen(id);
    } else if (m_type == ChooseSkill) {
        AH::Common::PropertyValueData::Property p = static_cast<AH::Common::PropertyValueData::Property> (ui->btnOptionActivate->property(OPTION_SKILL_PROPERTY).toInt());
        emit skillChosen(p);
    } else if (m_type == ChooseEncounter) {
        QString id = ui->btnOptionActivate->property(OPTION_ID_PROPERTY).toString();
        emit encounterChosen(id);
    }
}

QString OptionChooser::displayCosts(const Cost &costs)
{
    if (costs.getAlternatives().isEmpty()) {
        return "";
    }

    QStringList alts;
    foreach (CostList cl, costs.getAlternatives()) {
        QStringList itms;
        foreach (CostItem ci, cl) {
            itms << QString("%1 %2").arg(ci.amount).arg(Utils::stringForCostItem(ci.type));
        }
        alts << itms.join(" + ");
    }

    return "\n\nCosts:\n"+alts.join("\nOR\n");
}

void OptionChooser::setMoreWidget(QWidget *w)
{
    Q_ASSERT_X(ui->wgtMoreDescription->layout()->count() == 1, "OptionChooser::setMoreWidget", "Layout must only contain the spacer");
    QLayoutItem *itm = ui->wgtMoreDescription->layout()->takeAt(0);
    ui->wgtMoreDescription->layout()->addWidget(w);
    ui->wgtMoreDescription->layout()->addItem(itm);
    m_moreWgt = w;
}


void OptionChooser::OptionDescUpdater::objectDescribed(const DescribeObjectsData::ObjectDescription &desc)
{
    if (!btn.isNull()) {
        switch (desc.type) {
        case RequestObjectsData::Monster:
            btn->setProperty(OPTION_MONSTER_PROPERTY, desc.id);
            break;
        case RequestObjectsData::Object:
            btn->setProperty(OPTION_OBJECT_PROPERTY, desc.id);
            break;
        case RequestObjectsData::Gate:
            btn->setProperty(OPTION_GATE_PROPERTY, desc.id);
            break;
        default:
        {
            GameOptionData d;
            desc.data >> d;
            btn->setProperty(OPTION_DESCRIPTION_PROPERTY, d.description());
        }
        }
    }
    delete this;
}

OptionChooser::OptionDescUpdater::OptionDescUpdater(QPushButton *b)
    : btn(b)
{

}
