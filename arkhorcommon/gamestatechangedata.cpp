#include "gamestatechangedata.h"

namespace AH{ namespace Common {

void operator << (QVariant &v, const GameBoardChangeData::Movement &t) {
    QVariantMap m;
    m["id"] << t.id;
    //m["start"] << t.start;
    //m["end"] << t.end;
    m["path"] << t.path;
    v << m;
}

void operator >> (const QVariant &v, GameBoardChangeData::Movement &t) {
    QVariantMap m;
    v >> m;
    m["id"] >> t.id;
    //m["start"] >> t.start;
    //m["end"] >> t.end;
    m["path"] >> t.path;
}

void operator << (QVariant &v, const GameBoardChangeData::LocatedChange &t) {
    QVariantMap m;
    m["id"] << t.id;
    m["location"] << t.location;
    v << m;
}

void operator >> (const QVariant &v, GameBoardChangeData::LocatedChange &t) {
    QVariantMap m;
    v >> m;
    m["id"] >> t.id;
    m["location"] >> t.location;
}

void operator << (QVariant &v, const GameBoardChangeData::FieldChange &t) {
    QVariantMap m;
    m["location"] << t.location;
    m["clue"] << t.clue;
    m["lock"] << t.lock;
    m["unlock"] << t.unlock;
    m["seal"] << t.seal;
    m["unseal"] << t.unseal;
    m["eventNr"] << t.eventNr;
    v << m;
}

void operator >> (const QVariant &v, GameBoardChangeData::FieldChange &t) {
    QVariantMap m;
    v >> m;
    m["location"] >> t.location;
    m["clue"] >> t.clue;
    m["lock"] >> t.lock;
    m["unlock"] >> t.unlock;
    m["seal"] >> t.seal;
    m["unseal"] >> t.unseal;
    m["eventNr"] >> t.eventNr;
}

void operator << (QVariant &v, const GameBoardChangeData &t) {
    QVariantMap m;
    m["characterMovements"] << t.characterMovements;
    m["monsterMovements"] << t.monsterMovements;
    m["monsterAppear"] << t.monsterAppear;
    m["monsterDisappear"] << t.monsterDisappear;
    m["gateAppear"] << t.gateAppear;
    m["gateDisapper"] << t.gateDisappear;
    m["gateOpen"] << t.gateOpen;
    m["fieldChanges"] << t.fieldChanges;
    m["clearOutskirts"] << t.clearOutskirts;
    v << m;
}

void operator >> (const QVariant &v, GameBoardChangeData &t) {
    QVariantMap m;
    v >> m;
    m["characterMovements"] >> t.characterMovements;
    m["monsterMovements"] >> t.monsterMovements;
    m["monsterAppear"] >> t.monsterAppear;
    m["monsterDisappear"] >> t.monsterDisappear;
    m["gateAppear"] >> t.gateAppear;
    m["gateDisapper"] >> t.gateDisappear;
    m["gateOpen"] >> t.gateOpen;
    m["fieldChanges"] >> t.fieldChanges;
    m["clearOutskirts"] >> t.clearOutskirts;
}

}}
