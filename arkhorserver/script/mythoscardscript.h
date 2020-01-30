#ifndef MYTHOSCARDSCRIPT_H
#define MYTHOSCARDSCRIPT_H

#include <QObject>
#include <QScriptEngine>
#include "game/mythoscard.h"
#include <QScriptable>
#include "dynamicpropertyscript.h"

class QScriptContext;

class MythosCardScript : public DynamicScriptableObject, public MythosCard
{
    Q_OBJECT
public:
    explicit MythosCardScript(QObject *parent = 0);

    virtual QList<AH::Common::DynamicPropertyData> dynamicProperties() const;

    static void castFromValue(const QScriptValue &v, MythosCardScript *&o) { o = qobject_cast<MythosCardScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, MythosCardScript * const &in) { return eng->newQObject(in); }

    Q_PROPERTY(QString id READ id)
    /*
    Q_PROPERTY(AH::Common::MythosData::MythosType type READ type)
    Q_PROPERTY(AH::Common::FieldData::FieldID clueField READ clueField)
    Q_PROPERTY(AH::Common::FieldData::FieldID gateField READ gateField)
    */
    static MythosCardScript *createMythosCard(QScriptContext *ctx, QScriptEngine *eng);

    Q_INVOKABLE void removeFromGame() {/* NOOP */}

    Q_INVOKABLE void pass() { MythosCard::pass(); }
    Q_INVOKABLE void fail() { MythosCard::fail(); }

    virtual bool resolveDependencies();
    virtual void resolveDynamicAttributes();

    // Headlines
    virtual void executeHeadline();

    // Environment
    virtual PropertyModificationList getModifications() const;
    virtual GameOption *environmentFieldOption();
    virtual AH::Common::FieldData::FieldID environmentFieldId();
    virtual void activateEnvironment();
    virtual void onEndMovement();
    virtual bool onDefeatMonster(Character *byCharacter, Monster *m);
    virtual bool onAppearMonster(GameField *f, Monster *m);

    // Rumors
    virtual void onMythos();
    virtual void onPass();
    virtual void onFail();
    virtual void setupRumor();
    virtual void teardownRumor();
    virtual GameOption *rumorFieldOption();
    virtual AH::Common::FieldData::FieldID rumorFieldId();

signals:

public slots:

protected:
    virtual void dynamicPropertyChanged();

private:
    static bool verify(MythosCardScript *myth, QString *err = NULL);

    //static AH::Dimensions parseDimensions(QScriptValue v);

private:
    QScriptValue m_object;

    QScriptValue m_headlineFunc;

    GameOption *m_envFieldOption;
    QString m_envFieldOptionId;
    AH::Common::FieldData::FieldID m_envFieldId;
    PropertyModificationList m_envMods;
    QScriptValue m_onEndMoveFunc;
    QScriptValue m_onDefeatMonsterFunc;
    QScriptValue m_onAppearMonsterFunc;
    QScriptValue m_activateEnvFunc;

    QScriptValue m_setupRumorFunc;
    QScriptValue m_teardownRumorFunc;
    QScriptValue m_onMythosFunc;
    QScriptValue m_passFunc;
    QScriptValue m_failFunc;
    GameOption *m_rumorFieldOption;
    QString m_rumorFieldOptionId;
    AH::Common::FieldData::FieldID m_rumorFieldId;

#ifdef TEST_SCRIPT_BUILD
    friend class Game;
#endif
};

Q_DECLARE_METATYPE(MythosCardScript*)

#endif // MYTHOSCARDSCRIPT_H
