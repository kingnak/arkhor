#include "drawcardhelper.h"
#include "game.h"
#include "player.h"
#include "gameobject.h"
#include "arkhamencounter.h"
#include "otherworldencounter.h"
#include "gamecontext.h"
#include <choicedata.h>

DrawCardHelper::DrawCardHelper()
{
}

QList<GameObject *> DrawCardHelper::drawObjects(Player *p, QString desc, AH::GameObjectType type, int ct, int min, int max, bool allowQuickReturn, QString sourceId)
{
    QList<GameObject *> allObjs;
    QList<GameObject *> sels;

    // Normalize arguments:
    max = qMin(max, ct);
    min = qMin(min, max);
    if (max <= 0) return sels;
    if (ct <= 0) return sels;

    // Draw objects
    for (int i = 0; i < ct; ++i) {
        GameObject *o = gGame->drawObject(type);
        if (o) {
            allObjs << o;
        }
    }

	sels = doDrawObject(p, desc, allObjs, min, max, allowQuickReturn, sourceId);
    return sels;
}

QList<GameObject *> DrawCardHelper::drawMixedObjects(Player *p, QString desc, QList<AH::ObjectTypeCount> types, int min, int max, bool allowQuickReturn, QString sourceId)
{
    QList<GameObject *> sels;
    if (types.isEmpty()) return sels;

    QList<GameObject *> allObjs;
    foreach (AH::ObjectTypeCount t, types) {
        for (int i = 0; i < t.amount; ++i) {
            allObjs << gGame->drawObject(t.type);
        }
    }

	sels = doDrawObject(p, desc, allObjs, min, max, allowQuickReturn, sourceId);
    return sels;
}

ArkhamEncounter *DrawCardHelper::drawArkhamEncounter(Player *p, AH::Common::FieldData::FieldID fieldId, int ct)
{
    QList<ArkhamEncounter*> encs;
    for (int i = 0; i < ct; ++i) {
        auto e = gGame->drawArkhamEncounter(fieldId);
        if (e) encs << e;
    }
    for (auto e : encs) {
        gGame->returnEncounter(e);
    }

    if (encs.size() <= 1) return encs.value(0);

    QList<AH::Common::ChoiceData::OptionData> choices;
    for (int i = 0; i < encs.count(); ++i) {
        choices << AH::Common::ChoiceData::OptionData(QString::number(i), QString("Encounter %1").arg(i+1), encs[i]->description());
    }
	QString res = gGame->context().selectChoice(p, "Select Encounter", QString::null, choices, false);
    return encs.value(res.toInt());
}

OtherWorldEncounter *DrawCardHelper::drawOtherWorldEncounter(Player *p, AH::Common::FieldData::FieldID fieldId, int ct)
{
    QList<OtherWorldEncounter*> encs;
    for (int i = 0; i < ct; ++i) {
        auto e = gGame->drawOtherWorldEncounter(fieldId);
        if (e) encs << e;
    }
    for (auto e : encs) {
        gGame->returnEncounter(e);
    }

    if (encs.size() <= 1) return encs.value(0);

    QList<AH::Common::ChoiceData::OptionData> choices;
    for (int i = 0; i < encs.count(); ++i) {
        choices << AH::Common::ChoiceData::OptionData(QString::number(i), QString("Encounter %1").arg(i+1), encs[i]->description());
    }
	QString res = gGame->context().selectChoice(p, "Select Encounter", QString::null, choices, false);
    return encs.value(res.toInt());
}

QList<GameObject *> DrawCardHelper::doDrawObject(Player *p, QString desc, QList<GameObject *> avail, int min, int max, bool allowQuickReturn, QString sourceId)
{
    QList<GameObject *> sels;

    // Re-normalize arguments
    int ct = avail.count();
    max = qMin(max, ct);
    min = qMin(min, max);

    if (ct == 0) {
        return sels;
    }

    // Quick returns: Are those ok? More options?
    if (allowQuickReturn && ct == max && min == max) {
        return avail;
    }

    QStringList ids;
    foreach (GameObject *o, avail) {
        ids << o->id();
    }

    // Let user choose!
    AH::Common::ChoiceData choice;
    choice.setSelectObjects(ids, min, max);
    choice.setDescription(desc);
	choice.setSourceId(sourceId);
    AH::Common::ChoiceResponseData resp = p->offerChoice(choice);

    QStringList selIds;
    if (resp.isValid()) {
        selIds = resp.toStringList();
    }
    foreach (GameObject *o, avail) {
        if (selIds.contains(o->id())) {
            sels << o;
        } else {
            // Return it to the deck
            gGame->returnObject(o);
        }
    }
    return sels;
}
