#include "dierollwidget.h"
#include "ui_dierollwidget.h"
//#include "flowlayout.h"
#include <QtWidgets>
#include "objectregistry.h"
#include "utils.h"
#include "diewidget.h"

//#define PROPERTY_MODIFIER_ID "PROP_MOD_ID"
#define PROPERTY_OPTION_ID "PROP_OPT_ID"

using namespace AH::Common;

DieRollWidget::DieRollWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DieRollWidget)
{
    ui->setupUi(this);
    ui->wgtDice->setLayout(new QGridLayout);
    ui->scrlMods->setLayout(new QVBoxLayout);
    ui->wgtDieRollOpts->setLayout(new QVBoxLayout);
    connect(ui->lblDescription, SIGNAL(linkActivated(QString)), this, SLOT(requestObject(QString)));
}

DieRollWidget::~DieRollWidget()
{
    delete ui;
}

void DieRollWidget::displayDieRoll(AH::Common::DieRollTestData data)
{
    int ct = data.rollData().pool().dieCount()+data.rollData().pool().adjustment();

    QList<PropertyModificationData> mods = data.generalModifications();

    // Setup labels
    QString desc = data.description().trimmed();
    if (desc.isEmpty()) {
        desc = "Roll dies";
    }
    QString lnk = data.sourceId();
    if (!lnk.isEmpty()) {
        desc = QString("<a href=\"%1\">%2</a>").arg(lnk, desc.toHtmlEscaped());
    }

    ui->lblDescription->setText(desc);
    ui->lblSuccess->setVisible(data.isSucceeded());
    ui->lblAdjustment->setText(QString::number(data.rollData().pool().adjustment()));
    ui->lblDieCount->setText(QString::number(ct));

    if (data.rollData().pool().type() == DiePoolData::Property) {
        ui->lblBaseSkill->setText(Utils::stringForProperty(data.rollData().pool().property().property().property()));
        mods += data.rollData().pool().property().modifications();
    } else {
        ui->lblBaseSkill->setText("Fixed");
    }
    ui->lblBaseVal->setText(QString::number(data.rollData().pool().property().property().value()));
    if (data.type() == DieRollTestData::Boolean) {
        ui->lblTargetVal->setText(QString::number(data.targetValue()));
        QStringList s;
        foreach (qint32 i, data.rollData().successRolls()) {
            s << QString::number(i);
        }
        ui->lblSuccessVals->setText(s.join(", "));
    } else {
        ui->lblTargetVal->setText("-");
        ui->lblSuccessVals->setText("-");
    }

    // Display modifiers
    cleanModifiers();
    mods.append(data.clueBurnMods().modifications());
    foreach (PropertyModificationData mod, mods) {

        // TODO: This assumes modifier is already known
        QString id = mod.modifierId();
        QString name = id;
        if (ObjectRegistry::instance()->hasObject(id)) {
            AH::Common::GameOptionData obj;
            ObjectRegistry::instance()->getObject(id).data >> obj;
            name = obj.name();
        }

        QLabel *l = new QLabel(QString("<a href=\"%1\">%2</a>: %3").arg(id, name.toHtmlEscaped(), Utils::stringForPropertyModification(mod)));
        //l->setProperty(PROPERTY_MODIFIER_ID, mod.modifierId());
        l->setWordWrap(true);
        connect(l, SIGNAL(linkActivated(QString)), this, SLOT(requestObject(QString)));
        ui->scrlMods->layout()->addWidget(l);
    }
    // Spacing
    static_cast<QBoxLayout*>(ui->scrlMods->layout())->addStretch(1);

    // Display dice
    QList<quint32> vals = data.rollData().pool().dieValues();
    int totCt = qMax(ct, vals.size());

    cleanDice();

    QGridLayout *l = static_cast<QGridLayout *> (ui->wgtDice->layout());
    for (int i = 0; i < totCt; ++i) {
        //QLabel *lbl = new QLabel(QString::number(vals.value(i)), this);
        DieWidget *w = new DieWidget(this);
        w->setDieValue(vals.value(i));
        l->addWidget(w, i/2, i%2);
        //l->setAlignment(w, Qt::AlignRight);
    }

    m_clueBurnFactor = data.diceForClueBurn();

    // Set maximum value
    int clues = ObjectRegistry::instance()->thisCharacter().clues();
    ui->spnClueBurn->setMaximum(clues);
    ui->spnClueBurn->setValue(0);
    if (m_clueBurnFactor == 0) {
        ui->wgtDieAdd->setVisible(false);
    } else {
        ui->wgtDieAdd->setVisible(true);
    }

    // Display options
    cleanOptions();
    // Spacing
    static_cast<QBoxLayout*>(ui->wgtDieRollOpts->layout())->addStretch(1);
    foreach (AH::Common::DieRollTestData::OptionDescription opt, data.dieRollOptions()) {
        QPushButton *btn = new QPushButton(opt.second);
        btn->setProperty(PROPERTY_OPTION_ID, opt.first);
        connect(btn, SIGNAL(clicked()), this, SLOT(reRollOptionClicked()));
        ui->wgtDieRollOpts->layout()->addWidget(btn);
    }
}

void DieRollWidget::updateClueBurnAmount(int ct)
{
    ui->lblDieAddCount->setText(QString::number(ct*m_clueBurnFactor));
}

void DieRollWidget::on_btnOk_clicked()
{
    AH::Common::DieTestUpdateData upd(QString::null, ui->spnClueBurn->value());
    cleanDice();
    cleanModifiers();
    cleanOptions();
    emit dieUpdateChosen(upd);
}

void DieRollWidget::requestObject(QString id)
{
    //QString id = sender()->property(PROPERTY_MODIFIER_ID).toString();
    if (!id.isEmpty())
        emit itemInfoRequested(id);
}

void DieRollWidget::reRollOptionClicked()
{
    QString id = sender()->property(PROPERTY_OPTION_ID).toString();
    if (!id.isEmpty()) {
        AH::Common::DieTestUpdateData upd(id, 0);
        cleanDice();
        cleanModifiers();
        cleanOptions();
        emit dieUpdateChosen(upd);
    }
}

void DieRollWidget::cleanDice()
{
    QLayout *l = ui->wgtDice->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
    }
    foreach (QWidget *w, ui->wgtDice->findChildren<DieWidget*>()) {
        delete w;
    }
    delete ui->wgtDice->layout();
    QGridLayout *g = new QGridLayout;
    ui->wgtDice->setLayout(g);
    //g->addWidget(new QWidget, 0, 0);
    //g->setColumnStretch(0,1);
}

void DieRollWidget::cleanModifiers()
{
    QLayout *l = ui->scrlMods->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
    }
    foreach (QWidget *w, ui->scrlMods->findChildren<QLabel*>()) {
        delete w;
    }
}

void DieRollWidget::cleanOptions()
{
    QLayout *l = ui->wgtDieRollOpts->layout();
    if (l) {
        QLayoutItem *child;
        while ((child = l->takeAt(0)) != 0) {
            delete child;
        }
    }
    foreach (QWidget *w, ui->wgtDieRollOpts->findChildren<QPushButton*>()) {
        delete w;
    }
}
