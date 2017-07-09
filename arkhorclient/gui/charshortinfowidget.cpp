#include "charshortinfowidget.h"
#include "ui_charshortinfowidget.h"
#include "resourcepool.h"
#include <investigatordata.h>

CharShortInfoWidget::CharShortInfoWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharShortInfoWidget)
{
    ui->setupUi(this);
    ui->lblName->setFont(ResourcePool::instance()->loadMainFont());
    displayCharacter(nullptr);
}

CharShortInfoWidget::~CharShortInfoWidget()
{
    delete ui;
}

void CharShortInfoWidget::displayCharacter(const AH::Common::CharacterData *chr)
{
    if (chr) {
        ui->lblName->setText(chr->investigatorData().name());
        ui->lblSanity->setText(QString("%1 / %2").arg(chr->curSanity()).arg(chr->maxSanity()));
        ui->lblStamina->setText(QString("%1 / %2").arg(chr->curStamina()).arg(chr->maxStamina()));
        ui->lblClue->setText(QString::number(chr->clues()));
        ui->lblMoney->setText(QString::number(chr->money()));
        ui->lblSpeed->setText(QString::number(chr->investigatorData().attrSpeedSneak()[chr->attrSettings().value(0)].first));
        ui->lblSneak->setText(QString::number(chr->investigatorData().attrSpeedSneak()[chr->attrSettings().value(0)].second));
        ui->lblFight->setText(QString::number(chr->investigatorData().attrFightWill()[chr->attrSettings().value(1)].first));
        ui->lblWill->setText(QString::number(chr->investigatorData().attrFightWill()[chr->attrSettings().value(1)].second));
        ui->lblLore->setText(QString::number(chr->investigatorData().attrLoreLuck()[chr->attrSettings().value(2)].first));
        ui->lblLuck->setText(QString::number(chr->investigatorData().attrLoreLuck()[chr->attrSettings().value(2)].second));
    } else {
        ui->lblName->setText("?");
        ui->lblSanity->setText("?");
        ui->lblStamina->setText("?");
        ui->lblClue->setText("?");
        ui->lblMoney->setText("?");
        ui->lblSpeed->setText("?");
        ui->lblSneak->setText("?");
        ui->lblFight->setText("?");
        ui->lblWill->setText("?");
        ui->lblLore->setText("?");
        ui->lblLuck->setText("?");
    }
}
