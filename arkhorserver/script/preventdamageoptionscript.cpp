#include "preventdamageoptionscript.h"
#include <game/gameobject.h>
#include "gameobjectscript.h"
#include <script/gamescript.h>
#include <character.h>

PreventDamageOptionScript::PreventDamageOptionScript(const QString &id, const QString &desc, const QScriptValue &preventStamina, const QScriptValue &preventSanity, bool discard /* = false */, bool exhaust /* = true */, QObject *parent /* = 0 */)
    : QObject(parent),
      m_staVal(0),
      m_sanVal(0),
      m_discard(discard),
      m_exhaust(exhaust)
{
    this->m_id = id;
    this->m_baseDescription = desc;

    if (preventStamina.isFunction()) m_staFunc = preventStamina;
    else m_staVal = preventStamina.toInt32();

    if (preventSanity.isFunction()) m_sanFunc = preventSanity;
    else m_sanVal = preventSanity.toInt32();
}

bool PreventDamageOptionScript::execute(int damageStamina, int damageSanity)
{
    bool ret = true;

    QScriptValue obj = gGameScript->engine()->newQObject(dynamic_cast<GameObjectScript *> (m_source), QScriptEngine::QtOwnership, QScriptEngine::PreferExistingWrapperObject);

    if (damageStamina > 0) {
        if (m_staFunc.isFunction()) {
            QScriptValue res = gGameScript->call(GameScript::F_Prevent, m_staFunc, obj, QScriptValueList() << damageStamina);
            qDebug(qPrintable(res.toString()));
            m_staVal = res.toInt32();
        }
    }

    if (damageSanity > 0) {
        if (m_sanFunc.isFunction()) {
            QScriptValue res = gGameScript->call(GameScript::F_Prevent, m_sanFunc, obj, QScriptValueList() << damageSanity);
            qDebug(qPrintable(res.toString()));
            m_sanVal = res.toInt32();
        }
    }

    if (m_source) {
        m_source->preventDamage();
        if (m_exhaust) {
            m_source->exhaust();
        }
        if (m_discard) {
            m_source->owner()->removeFromInventory(m_source);
        }
    }

    setSource(nullptr);
    return ret;
}

int PreventDamageOptionScript::preventedStamina() const
{
    return m_staVal;
}

int PreventDamageOptionScript::preventedSanity() const
{
    return m_sanVal;
}

bool PreventDamageOptionScript::canPreventStamina() const
{
    if (m_staFunc.isFunction()) return true;
    return m_staVal != 0;
}

bool PreventDamageOptionScript::canPreventSanity() const
{
    if (m_sanFunc.isFunction()) return true;
    return m_sanVal != 0;
}
