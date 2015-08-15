#ifndef INVESTIGATORSCRIPT_H
#define INVESTIGATORSCRIPT_H

#include <QObject>
#include <QScriptValue>
#include "game/investigator.h"

class QScriptContext;

class InvestigatorScript : public QObject, public Investigator
{
    Q_OBJECT
public:
    explicit InvestigatorScript(QObject *parent = 0);

    Q_PROPERTY(QString id READ id)
    Q_PROPERTY(QString uniqueAbilityId READ uniqueAbilityId)

    virtual Character *instantiate();

    static QScriptValue createInvestigator(QScriptContext *ctx, QScriptEngine *eng);

    // For pre-game display
    virtual QString uniqueAbilityName() const;
    virtual QString uniqueAbilityDescription() const;
    virtual QStringList fixedPossessionNames() const;

signals:

public slots:

private:
    static bool parseAttributeList(QScriptValue v, QList<AH::Common::InvestigatorData::AttributeValuePair> &ret);
    static bool verify(InvestigatorScript *inv, QString *msg = NULL);
};

#endif // INVESTIGATORSCRIPT_H
