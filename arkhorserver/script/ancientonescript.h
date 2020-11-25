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

    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(int attackAdjustment READ attackAdjustment)
    Q_PROPERTY(int doomTrack READ doomValue)
    Q_PROPERTY(int maxDoomTrack READ doomTrack)

    void awake() override;

    Q_INVOKABLE void increaseDoomTrack(int amount = 1) { AncientOne::increaseDoomTrack(amount); }
    Q_INVOKABLE void decreaseDoomTrack(int amount = 1) { AncientOne::increaseDoomTrack(-amount); }
    Q_INVOKABLE void setCombatAdjustment(int adjustment) { m_combatAdjustment = adjustment; }
    Q_INVOKABLE void decreaseAttackAdjustment(int amount = 1) override { AncientOne::decreaseAttackAdjustment(amount); }

    PropertyModificationList getSlumberModifications() const override { return m_slumberModifications; }

    bool onUnconscious(Character *c) override;
    bool onInsane(Character *c) override;
    bool onLostInSpaceAndTime(Character *c) override;
    bool onDefeatMonster(Character *c, Monster *m) override;

    void onEndMythos() override;
    void attack() override;
    bool postAttack() override;
    QList<AH::Common::MythosData::EnvironmentType> ignoreEnvironmentTypes() const override { return m_ignEnvTypes; }

    static QList<AH::Common::MythosData::EnvironmentType> parseIgnoreEnvironmentTypes(const QScriptValue &v);

signals:

public slots:

private:
    static bool verify(AncientOneScript *m, QString *msg = NULL);

private:
    QScriptValue m_onEndMythosFunc;
    QScriptValue m_awakeFunc;
    QScriptValue m_attackFunc;
    QScriptValue m_postAttackFunc;
    QScriptValue m_this;

    QScriptValue m_onUnconsciousFunc;
    QScriptValue m_onInsaneFunc;
    QScriptValue m_onLostFunc;
    QScriptValue m_onDefeatMonsterFunc;

    PropertyModificationList m_slumberModifications;
    QList<AH::Common::MythosData::EnvironmentType> m_ignEnvTypes;
};

Q_DECLARE_METATYPE(AncientOneScript*)

#endif // ANCIENTONESCRIPT_H
