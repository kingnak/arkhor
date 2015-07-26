#ifndef ANCIENTONESCRIPT_H
#define ANCIENTONESCRIPT_H

#include <QObject>
#include <QScriptValue>
#include <QScriptable>
#include <QScriptEngine>
#include "ancientone.h"

class QScriptContext;

class AncientOneScript : public QObject, public AncientOne
{
    Q_OBJECT
public:
    explicit AncientOneScript(QObject *parent = 0);

    static AncientOneScript *createAncientOne(QScriptContext *ctx, QScriptEngine *eng);

    static void castFromValue(const QScriptValue &v, AncientOneScript *&o) { o = qobject_cast<AncientOneScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, AncientOneScript * const &in) { return eng->newQObject(in); }

    Q_PROPERTY(int attackAdjustment READ attackAdjustment)

    Q_INVOKABLE void awake();

    Q_INVOKABLE void increaseDoomTrack(int amount = 1) { AncientOne::increaseDoomTrack(amount); }
    Q_INVOKABLE void decreaseDoomTrack(int amount = 1) { AncientOne::increaseDoomTrack(-amount); }
    Q_INVOKABLE void setCombatAdjustment(int adjustment) { m_combatAdjustment = adjustment; }
    Q_INVOKABLE void decreaseAttackAdjustment(int amount = 1) { AncientOne::decreaseAttackAdjustment(amount); }

    virtual PropertyModificationList getSlumberModifications() const { return m_slumberModifications; }
    virtual PropertyModificationList getMonsterModifications(QString typeId) const;

    virtual void attack();

signals:

public slots:

private:
    static bool verify(AncientOneScript *m, QString *msg = NULL);

private:
    QScriptValue m_awakeFunc;
    QScriptValue m_attackFunc;
    QScriptValue m_this;

    PropertyModificationList m_slumberModifications;
    QMap<QString, PropertyModificationList> m_monsterModifications;

};

Q_DECLARE_METATYPE(AncientOneScript*)

#endif // ANCIENTONESCRIPT_H
