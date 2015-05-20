#include "optionchooser.h"
#include "ui_optionchooser.h"
#include <QtGui>
#include "flowlayout.h"
#include "ahmaingui.h"

using namespace AH::Common;

static const char *OPTION_DESCRIPTION_PROPERTY = "DESCRIPTION";
static const char *OPTION_ID_PROPERTY = "ID";
static const char *OPTION_SKILL_PROPERTY = "SKILL";

OptionChooser::OptionChooser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OptionChooser)
{
    ui->setupUi(this);
    ui->wgtOptionsList->setLayout(new FlowLayout);
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
        b->setProperty(OPTION_DESCRIPTION_PROPERTY, o.description());
        b->setProperty(OPTION_ID_PROPERTY, o.id());
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
        QString name = AhMainGui::stringForProperty(v.property().property());
        int val = v.finalVal();
        QPushButton *btn = new QPushButton(name);
        btn->setProperty(OPTION_DESCRIPTION_PROPERTY, QString("Skill %1. Current Value: %2").arg(name).arg(val));
        btn->setProperty(OPTION_SKILL_PROPERTY, QVariant::fromValue(static_cast<qint32>(v.property().property())));
        connect(btn, SIGNAL(clicked()), this, SLOT(showOption()));
        l->addWidget(btn);
    }
}

void OptionChooser::cleanupOptions()
{
    QLayout *l = ui->wgtOptionsList->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
        //ui->wgtOptionsList->setLayout(NULL);
        //delete l;
    }
    foreach (QWidget *w, ui->wgtOptionsList->findChildren<QWidget*>()) {
        delete w;
    }

    ui->lblOptionDescription->setText("");
    ui->btnOptionActivate->setEnabled(false);
}

void OptionChooser::showOption()
{
    ui->lblOptionDescription->setText(sender()->property(OPTION_DESCRIPTION_PROPERTY).toString());
    ui->btnOptionActivate->setProperty(OPTION_ID_PROPERTY, sender()->property(OPTION_ID_PROPERTY));
    ui->btnOptionActivate->setProperty(OPTION_SKILL_PROPERTY, sender()->property(OPTION_SKILL_PROPERTY));
    ui->btnOptionActivate->setEnabled(true);
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
    }
}
