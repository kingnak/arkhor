#include "characterscript.h"

CharacterScript::CharacterScript(Investigator *i, QObject *parent) :
    QObject(parent), Character(i)
{
}
