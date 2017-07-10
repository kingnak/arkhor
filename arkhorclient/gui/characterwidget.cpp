#include "characterwidget.h"
#include "ui_characterwidget.h"
#include "../utils.h"
#include "objectregistry.h"

CharacterWidget::CharacterWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CharacterWidget)
{
    ui->setupUi(this);
}

CharacterWidget::~CharacterWidget()
{
    delete ui;
}

void CharacterWidget::objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc)
{
    if (desc.id == m_curCharId && desc.type == AH::Common::RequestObjectsData::Character) {
        AH::Common::CharacterData c;
        desc.data >> c;
        updateCharacterData(&c);
    }
}

void CharacterWidget::displayCharacterData(const AH::Common::CharacterData *data)
{
    m_curCharId = QString::null;
    ObjectRegistry::instance()->unsubscribe(this);
    updateCharacterData(data);
    if (data) {
        m_curCharId = data->id();
        ObjectRegistry::instance()->asyncSubscribeObject(this, data->id(), AH::Common::RequestObjectsData::Character);
    }
}

void CharacterWidget::updateCharacterData(const AH::Common::CharacterData *data)
{
    if (data) {
        ui->lblName->setText(data->investigatorData().name());
        ui->lblClue->setText(QString::number(data->clues()));
        ui->lblStamina->setText(QString("%1 / %2").arg(data->curStamina()).arg(data->maxStamina()));
        ui->lblSanity->setText(QString("%1 / %2").arg(data->curSanity()).arg(data->maxSanity()));
        ui->lblMoney->setText(QString::number(data->money()));
        AH::Common::InvestigatorData::AttributeValuePair ss = data->investigatorData().attrSpeedSneak().value(data->attrSettings().value(0));
        ui->lblSpeedSneak->setText(QString("%1 / %2").arg(ss.first).arg(ss.second));
        AH::Common::InvestigatorData::AttributeValuePair fw = data->investigatorData().attrFightWill().value(data->attrSettings().value(1));
        ui->lblFightWill->setText(QString("%1 / %2").arg(fw.first).arg(fw.second));
        AH::Common::InvestigatorData::AttributeValuePair ll = data->investigatorData().attrLoreLuck().value(data->attrSettings().value(2));
        ui->lblLuckLore->setText(QString("%1 / %2").arg(ll.first).arg(ll.second));
        ui->lblFocus->setText(QString::number(data->curFocus()));
        ui->lblMonsterTrophies->setText(QString::number(data->monsterMarkerIds().count()));
        ui->lblGateTrophies->setText(QString::number(data->gateMarkerIds().count()));
        ui->lblMonsterToughness->setText(QString::number(data->getMonsterToughness()));
        ui->lblField->setText(Utils::stringForField(data->fieldId()));
    } else {
        ui->lblName->setText("");
        ui->lblClue->setText("");
        ui->lblStamina->setText("");
        ui->lblSanity->setText("");
        ui->lblMoney->setText("");
        ui->lblSpeedSneak->setText("");
        ui->lblFightWill->setText("");
        ui->lblLuckLore->setText("");
        ui->lblFocus->setText("");
        ui->lblMonsterTrophies->setText("");
        ui->lblGateTrophies->setText("");
        ui->lblMonsterToughness->setText("");
        ui->lblField->setText("");
    }
}
