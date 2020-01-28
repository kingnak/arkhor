#include "gamefieldscript.h"
#include "game/game.h"
#include "characterscript.h"
#include "gamescript.h"
#include "gatescript.h"
#include <QDebug>

GameFieldScript::GameFieldScript(QObject *parent) :
    QObject(parent), m_field(NULL)
{
}

void GameFieldScript::lockUntilNextRound()
{
    Q_ASSERT(m_field);
    quint32 lockFlag = gGame->context().phase();
    m_field->lock(lockFlag);
}

QScriptValue GameFieldScript::nearestFieldsWithCharacters()
{
    return nearestFieldsWithCharacters({});
}

QScriptValue GameFieldScript::nearestFieldsWithCharacters(QScriptValue exceptFields)
{
   return nearestFieldsWith(exceptFields, [](GameField *f) -> bool {
       return !f->characters().isEmpty();
   });
}

QScriptValue GameFieldScript::nearestGates()
{
    if (m_field->type() == AH::Common::FieldData::FieldType::OtherWorld) {
        // technically, its the back fields. But that can lead to unexpected things.
        // instead, backFields should be called directly on OtherWorld fields
        return GameScript::makeArray(QList<int>());
    }

    QList<GameField *> fields = gGame->neareastFieldsWith(m_field, [](GameField *f) -> bool {
        return f->gate() != nullptr;
    });

    QList<GateScript *> gates;
    for (auto f : fields) {
        gates << new GateScript(f->gate());
    }
    shuffle_list(gates);
    return GameScript::makeArray(gates);
}

QScriptValue GameFieldScript::nearestFieldsWith(QScriptValue exceptFields, std::function<bool (GameField *)> predicate)
{
    QList<AH::Common::FieldData::FieldID> except;
    if (exceptFields.isArray()) {
        except = GameScript::array2TypedList<AH::Common::FieldData::FieldID>(exceptFields);
    } else if (exceptFields.isNumber()) {
        except << static_cast<AH::Common::FieldData::FieldID> (exceptFields.toInt32());
    }

    QList<GameField *> ret = gGame->neareastFieldsWith(m_field, predicate, except);
    QList<qint32> retIds;
    for (auto f : ret) retIds << f->id();
    shuffle_list(retIds);
    return GameScript::makeArray(retIds);
}

QObjectList GameFieldScript::characters()
{
    Q_ASSERT(m_field);
    QObjectList ret;
    foreach (Character *c, m_field->characters()) {
        CharacterScript *cs = dynamic_cast<CharacterScript *> (c);
        if (cs) {
            ret << cs;
        } else {
            qCritical() << "Non-Script Character detected in GameContextScript::getAllCharacters";
        }
    }

    return ret;
}


