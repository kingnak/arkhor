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
    virtual PropertyModificationList getModifications();

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

private:
    static bool verify(MythosCardScript *myth, QString *err = NULL);

    //static AH::Dimensions parseDimensions(QScriptValue v);

private:
    QScriptValue m_object;

    QScriptValue m_headlineFunc;
    QScriptValue m_setupRumorFunc;
    QScriptValue m_teardownRumorFunc;
    QScriptValue m_onMythosFunc;
    QScriptValue m_passFunc;
    QScriptValue m_failFunc;

    GameOption *m_rumorFieldOption;
    QString m_rumorFieldOptionId;
    AH::Common::FieldData::FieldID m_rumorFieldId;
};

Q_DECLARE_METATYPE(MythosCardScript*)

#endif // MYTHOSCARDSCRIPT_H
