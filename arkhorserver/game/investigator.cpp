#include "investigator.h"
#include "character.h"
#include "gameobject.h"
#include <QDebug>

Investigator::Investigator()
    : m_isInstantiated(false)
{
}

Character *Investigator::instantiate()
{
    if (m_isInstantiated) return nullptr;
    Character *c = new Character(this);
    c->instantiateFromInvestigator();
    m_isInstantiated = true;
    return c;
}

bool Investigator::resolveDependecies(GameRegistry *reg)
{
    bool ok = true;
    if (!m_uniqueAbilityId.isEmpty()) {
        if (auto ua = reg->findObjectById(m_uniqueAbilityId)) {
            m_uniqueAbilityName = ua->name();
            m_uniqueAbilityDesc = ua->description();
        } else {
            qWarning() << "Cannot resolve unique ability" << m_uniqueAbilityId << "for character" << id();
            ok = false;
        }
    }

    for (const auto &id : m_fixedPossesionObjectIds) {
        if (reg->findObjectPrototypeByType(id) == nullptr) {
            qWarning() << "Cannot resolve fixed possession" << id << "for character" << this->id();
            ok = false;
        }
    }

    return ok;
}
