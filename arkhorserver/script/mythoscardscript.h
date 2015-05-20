#ifndef MYTHOSCARDSCRIPT_H
#define MYTHOSCARDSCRIPT_H

#include <QObject>
#include <QScriptEngine>
#include "game/mythoscard.h"
#include <QScriptable>

class QScriptContext;

class MythosCardScript : public QObject, public MythosCard
{
    Q_OBJECT
public:
    explicit MythosCardScript(QObject *parent = 0);

    static void castFromValue(const QScriptValue &v, MythosCardScript *&o) { o = qobject_cast<MythosCardScript *> (v.toQObject()); }
    static QScriptValue castToValue(QScriptEngine *eng, MythosCardScript * const &in) { return eng->newQObject(in); }

    Q_PROPERTY(QString id READ id)
    /*
    Q_PROPERTY(AH::Common::MythosData::MythosType type READ type)
    Q_PROPERTY(AH::Common::FieldData::FieldID clueField READ clueField)
    Q_PROPERTY(AH::Common::FieldData::FieldID gateField READ gateField)
    */
    static MythosCardScript *createMythosCard(QScriptContext *ctx, QScriptEngine *eng);

    virtual void resolveDynamicAttributes();

signals:

public slots:

private:
    static bool verify(MythosCardScript *myth, QString *err = NULL);

    //static AH::Dimensions parseDimensions(QScriptValue v);

private:
    QScriptValue m_object;
};

Q_DECLARE_METATYPE(MythosCardScript*)

#endif // MYTHOSCARDSCRIPT_H
