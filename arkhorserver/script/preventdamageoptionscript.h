#ifndef PREVENTDAMAGEOPTIONSCRIPT_H
#define PREVENTDAMAGEOPTIONSCRIPT_H

#include <QObject>
#include <QScriptValue>
#include <game/actions/preventdamageaction.h>

class PreventDamageOptionScript : public QObject, public PreventDamageOption
{
    Q_OBJECT
public:
    explicit PreventDamageOptionScript(const QString &id, const QString &desc, const QScriptValue &preventStamina, const QScriptValue &preventSanity, bool discard = false, bool exhaust = true, QObject *parent = 0);

    bool execute(int damageStamina, int damageSanity) override;
    int preventedStamina() const override;
    int preventedSanity() const override;

    bool canPreventStamina() const override;
    bool canPreventSanity() const override;

private:
    QScriptValue m_staFunc;
    QScriptValue m_sanFunc;
    int m_staVal;
    int m_sanVal;

    bool m_discard;
    bool m_exhaust;
};

#endif // PREVENTDAMAGEOPTIONSCRIPT_H
