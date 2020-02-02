#include "fieldinfowidget.h"
#include "ui_fieldinfowidget.h"
#include "utils.h"
#include "objectregistry.h"
#include <gatedata.h>

using namespace AH::Common;

FieldInfoWidget::FieldInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FieldInfoWidget)
{
    ui->setupUi(this);
    auto p = ui->txtSpecialOpts->palette();
    p.setColor(QPalette::ColorGroup::All, QPalette::ColorRole::Base, p.color(QPalette::ColorGroup::Disabled, QPalette::ColorRole::Base));
    ui->txtSpecialOpts->setPalette(p);
    ui->scrlChars->setLayout(new QVBoxLayout);
    ui->scrlMonsters->setLayout(new QVBoxLayout);
    ui->scrlAdditionalOpts->setLayout(new QVBoxLayout);

    connect(ui->txtGate, SIGNAL(linkActivated(QString)), this, SIGNAL(objectDescriptionRequested(QString)));
}

FieldInfoWidget::~FieldInfoWidget()
{
    delete ui;
}

void FieldInfoWidget::clear()
{
    ui->txtName->clear();
    ui->txtType->clear();
    ui->txtClues->clear();
    ui->txtGate->clear();
    ui->txtState->clear();
    ui->txtSpecialOpts->clear();
    cleanList(ui->scrlChars);
    cleanList(ui->scrlMonsters);
    cleanList(ui->scrlAdditionalOpts);
    ui->lblGate->setVisible(true);
    ui->txtGate->setVisible(true);
    ui->lblClues->setVisible(true);
    ui->txtClues->setVisible(true);
    ui->lblState->setVisible(true);
    ui->txtState->setVisible(true);
    ui->grpSpecialOpts->setVisible(true);
    ui->grpChars->setVisible(true);
    ui->grpAdditionalOpts->setVisible(true);
    ui->grpMonsters->setVisible(true);
}

void FieldInfoWidget::displayField(AH::Common::GameFieldData fd)
{
    clear();
    ui->txtName->setText(Utils::stringForField(fd.id()));
    ui->txtType->setText(Utils::stringForFieldType(fd.type()));
    ui->txtClues->setText(QString::number(fd.clueAmount()));
    if (!fd.gateId().isEmpty()) {
        GateData g = ObjectRegistry::instance()->getObject<GateData>(fd.gateId());
        ui->txtGate->setText(QString("<a href=\"%1\">%2</a>").arg(g.id(), g.isOpen() ? Utils::stringForField(g.destination()) : "?"));
    }

    QStringList states;
    if (fd.isLocked()) states << "Locked";
    if (fd.isSealed()) states << "Sealed";
    ui->txtState->setText(states.join(", "));

    for (auto cId : fd.characterIds()) {
        auto c = ObjectRegistry::instance()->getObject<CharacterData>(cId);
        addToList(ui->scrlChars, c.investigatorData().name(), cId);
    }
    addStretch(ui->scrlChars);

    for (auto mId : fd.monsterIds()) {
        auto m = ObjectRegistry::instance()->getObject<MonsterData>(mId);
        addToList(ui->scrlMonsters, m.name(), mId);
    }
    addStretch(ui->scrlMonsters);

    auto fdd = ObjectRegistry::instance()->getFieldDescription(fd.id());
    for (auto opt : fdd.additionalOptions) {
        addToList(ui->scrlAdditionalOpts, opt.name, opt.detail);
    }
    addStretch(ui->scrlAdditionalOpts);
    if (fdd.additionalOptions.empty()) {
        ui->grpAdditionalOpts->setVisible(false);
    }

    QStringList fieldOptDescs;
    for (auto opt : fdd.fieldOptions) {
        fieldOptDescs << QString("<p><b>%1</b><br/>%2</p>").arg(opt.name, opt.detail);
    }
    ui->txtSpecialOpts->setHtml(fieldOptDescs.join('\n'));
    if (fdd.fieldOptions.empty()) {
        ui->grpSpecialOpts->setVisible(false);
    }

    switch (fd.type()) {
    case FieldData::FieldType::Sky:
    case FieldData::FieldType::Outskirts:
        ui->lblGate->setVisible(false);
        ui->txtGate->setVisible(false);
        ui->lblClues->setVisible(false);
        ui->txtClues->setVisible(false);
        ui->lblState->setVisible(false);
        ui->txtState->setVisible(false);
        ui->grpSpecialOpts->setVisible(false);
        ui->grpChars->setVisible(false);
        ui->grpAdditionalOpts->setVisible(false);
        break;
    case FieldData::FieldType::Street:
        ui->lblGate->setVisible(false);
        ui->txtGate->setVisible(false);
        ui->lblClues->setVisible(false);
        ui->txtClues->setVisible(false);
        ui->lblState->setVisible(false);
        ui->txtState->setVisible(false);
        break;
    case FieldData::FieldType::OtherWorld:
    case FieldData::FieldType::SpaceAndTime:
        ui->lblGate->setVisible(false);
        ui->txtGate->setVisible(false);
        ui->lblClues->setVisible(false);
        ui->txtClues->setVisible(false);
        ui->lblState->setVisible(false);
        ui->txtState->setVisible(false);
        ui->grpSpecialOpts->setVisible(false);
        ui->grpAdditionalOpts->setVisible(false);
        ui->grpMonsters->setVisible(false);
        break;
    case FieldData::FieldType::Location:
        break;
    }
}

void FieldInfoWidget::cleanList(QScrollArea *list)
{
    QLayout *l = list->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != nullptr) {
            delete child;
        }
    }
    for (QWidget *w : list->findChildren<QLabel*>()) {
        delete w;
    }
}

void FieldInfoWidget::addToList(QScrollArea *list, QString name, QString id)
{
    QLabel *l = new QLabel(QString("<a href=\"%1\">%2</a>").arg(id, name.toHtmlEscaped()));
    l->setWordWrap(true);
    connect(l, SIGNAL(linkActivated(QString)), this, SIGNAL(objectDescriptionRequested(QString)));
    list->layout()->addWidget(l);
}

void FieldInfoWidget::addStretch(QScrollArea *list)
{
    static_cast<QBoxLayout*>(list->layout())->addStretch(1);
}


