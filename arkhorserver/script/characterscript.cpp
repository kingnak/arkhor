#include "characterscript.h"
#include <QDebug>
#include "gamescript.h"
#include "investigatorscript.h"

CharacterScript::CharacterScript(Investigator *i, QObject *parent) :
    QObject(parent), Character(i)
{
    m_fieldBridge = new GameFieldScript(this);
}

bool CharacterScript::hasObject(QString typeId)
{
    foreach (GameObject *o, inventory()) {
        if (o->typeId() == typeId) {
            return true;
        }
    }
    return false;
}

GameFieldScript *CharacterScript::fieldScript()
{
    m_fieldBridge->setField(m_field);
    return m_fieldBridge;
}

int CharacterScript::getSkillValue(AH::Skill skill)
{
    return gGame->context().getCharacterSkill(this, skill).finalVal();
}

bool CharacterScript::placeOnField(AH::Common::FieldData::FieldID fieldId)
{
    GameField *field = gGame->board()->field(fieldId);
    if (field) {
        field->placeCharacter(this);
        return true;
    }
    return false;
}

QString CharacterScript::selectChoice(QString desc, const QString &sourceId, QList<AH::Common::ChoiceData::OptionData> options)
{
    return selectChoice(desc, sourceId, options, false);
}

QString CharacterScript::selectChoice(QString desc, const QString &sourceId, QList<AH::Common::ChoiceData::OptionData> options, bool canBeCanceled)
{
    return gGame->context().selectChoice(gGame->playerForCharacter(this), desc, sourceId, options, canBeCanceled);
}

bool CharacterScript::canCloseGate()
{
    if (m_explorededGate && m_explorededGate == gGame->context().gate()) {
        return true;
    }
    return false;
}

int CharacterScript::genericDieRollSum(QString desc, QString sourceId, int dieCount)
{
    DieTestHelper::DieTestSpec spec = DieTestHelper::createGenericSummer(desc, sourceId, dieCount);
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->playerForCharacter(this), spec);
    return res.intResult;
}

bool CharacterScript::genericDieRollTest(QString desc, QString sourceId, int dieCount, int target)
{
    int ct = genericDieRollCount(desc, sourceId, dieCount);
    return ct >= target;
}

int CharacterScript::genericDieRollCount(QString desc, QString sourceId, int dieCount)
{
    QList<quint32> successes;
    for (int i = gGame->context().getCharacterProperty(this, PropertyValue::Prop_MinSuccessDieRoll).finalVal(); i <= 6; ++i) {
        successes << i;
    }
    DieTestHelper::DieTestSpec spec = DieTestHelper::createGenericCounter(desc, sourceId, dieCount, successes);
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->playerForCharacter(this), spec);
    return res.intResult;
}

bool CharacterScript::skillTest(QString desc, QString sourceId, AH::Skill skill, int adjust, int target)
{
    DieTestHelper::DieTestSpec spec = DieTestHelper::createSkillTest(desc, sourceId, this, skill, adjust, target);
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->playerForCharacter(this), spec);
    return res.boolResult;
}

int CharacterScript::dieRollSkillCount(QString desc, QString sourceId, AH::Skill skill, int adjust)
{
    DieTestHelper::DieTestSpec spec = DieTestHelper::createSkillCounter(desc, sourceId, this, skill, adjust);
    DieTestHelper::DieTestResult res = DieTestHelper::executeDieTest(gGame->playerForCharacter(this), spec);
    return res.intResult;
}

bool CharacterScript::onUnconscious()
{
    if (m_unconsciousFunc.isFunction()) {
        QScriptValue res = gGameScript->call(GameScript::F_Character, m_unconsciousFunc, gGameScript->getCharacter(this));
        if (res.isBool())
            return res.toBool();
    }
    return true;
}

bool CharacterScript::onInsane()
{
    if (m_insaneFunc.isFunction()) {
        QScriptValue res = gGameScript->call(GameScript::F_Character, m_insaneFunc, gGameScript->getCharacter(this));
        if (res.isBool())
            return res.toBool();
    }
    return true;
}

bool CharacterScript::onLostInSpaceAndTime()
{
    if (m_lostFunc.isFunction()) {
        QScriptValue res = gGameScript->call(GameScript::F_Character, m_lostFunc, gGameScript->getCharacter(this));
        if (res.isBool())
            return res.toBool();
    }
    return true;
}

void CharacterScript::instantiateFromInvestigator()
{
    Character::instantiateFromInvestigator();
    auto is = dynamic_cast<InvestigatorScript*> (m_investigator);
    m_unconsciousFunc = is->m_unconsciousFunc;
    m_insaneFunc = is->m_insaneFunc;
    m_lostFunc = is->m_lostFunc;
}

QObjectList CharacterScript::getInventoryScript() const
{
    QObjectList ret;
    foreach (GameObject *o, m_inventory) {
        GameObjectScript *os = dynamic_cast<GameObjectScript *> (o);
        if (os) {
            ret << os;
        } else {
            qCritical() << "Non-Script Object detected in CharacterScript::getInventoryScript";
        }
    }

    return ret;
}
