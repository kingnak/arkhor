#ifndef PREVENTDAMAGEOPTIONSCRIPT_H
#define PREVENTDAMAGEOPTIONSCRIPT_H

#include <QObject>
#include <QScriptValue>
#include <game/actions/preventdamageaction.h>

class PreventDamageOptionScript : public QObject, public PreventDamageOption
{
    Q_OBJECT
public:
    explicit PreventDamageOptionScript(const QString &id, const QString &desc, QScriptValue preventStamina, QScriptValue preventSanity, bool discard = false, bool exhaust = true, QObject *parent = 0);

    virtual bool execute(int damageStamina, int damageSanity);
    virtual int preventedStamina() const;
    virtual int preventedSanity() const;

    virtual bool canPreventStamina() const;
    virtual bool canPreventSanity() const;

private:
    QScriptValue m_staFunc;
    QScriptValue m_sanFunc;
    int m_staVal;
    int m_sanVal;

    bool m_discard;
    bool m_exhaust;
};

#endif // PREVENTDAMAGEOPTIONSCRIPT_H
