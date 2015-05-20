#ifndef CASTSPELLACTION_H
#define CASTSPELLACTION_H

#include "game/gameaction.h"
#include "game/gameoption.h"
#include "game/gameobject.h"

/*
class CastSpellAction : public GameActionSc
{
public:
    CastSpellAction(GameObject *obj);

    //virtual QString id() const { return "AC_CAST_"+m_spell->id(); }
    virtual QString name() const { return "Cast "+m_spell->name(); }
    virtual QString description() const { return m_spell->description(); }

    virtual bool execute();

private:
    GameObject *m_spell;
    friend class CastSpellOption;
};
*/
/*
class CastSpellOption : public GameOption
{
public:
    CastSpellOption(GameObject *obj)
        : csa(obj) {}

    QString sourceId() const { return csa.m_spell->id(); }

private:
    CastSpellAction csa;
};
*/
#endif // CASTSPELLACTION_H
