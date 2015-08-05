#include "monstermodifierscript.h"
#include "gamescript.h"
#include "propertymodificationscript.h"

using namespace AH::Common;

bool MonsterModifierScript::parseMonsterModifications(QScriptValue data, MonsterModifier &mod, const PropertyModifier *propMod)
{
    QScriptValue monsterMods = data.property("monsterModifications");
    if (monsterMods.isValid() && !monsterMods.isUndefined()) {
        QScriptValueList mmLst = GameScript::array2list(monsterMods);
        foreach (QScriptValue mm, mmLst) {
            QScriptValue mods = mm.property("mod");
            PropertyModificationList lst;
            if (!PropertyModificationScript::parsePropertyModificationList(propMod, mods, lst)) {
                return false;
            }

            if (mm.property("id").isString()) {
                QString id = mm.property("id").toString();
                mod.addModifications(id, lst);
            } else if (mm.property("attributes").isNumber()) {
                MonsterData::MonsterAttributes attrs = static_cast<MonsterData::MonsterAttributes>
                        (mm.property("attributes").toInt32());
                mod.addModifications(attrs, lst);
            } else {
                return false;
            }
            //ret->m_monsterModifications[id] = lst;
        }
    }

    QScriptValue monsterMoveMods = data.property("monsterMoveModifications");
    if (monsterMoveMods.isValid() && !monsterMoveMods.isUndefined()) {
        QScriptValueList mmLst = GameScript::array2list(monsterMoveMods);
        foreach (QScriptValue mm, mmLst) {
            if (!mm.property("moveAs").isValid() || !mm.property("moveAs").isNumber()) {
                return false;
            }

            MonsterData::MovementType moveType = static_cast<MonsterData::MovementType> (mm.property("moveAs").toInt32());

            if (mm.property("id").isString()) {
                QString id = mm.property("id").toString();
                mod.addMoveModification(id, moveType);
            } else if (mm.property("type").isNumber()) {
                MonsterData::MovementType oldType = static_cast<MonsterData::MovementType> (mm.property("type").toInt32());
                mod.addMoveModification(oldType, moveType);
            } else {
                return false;
            }
        }
    }

    return true;
}
