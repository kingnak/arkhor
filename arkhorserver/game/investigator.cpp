#include "investigator.h"
#include "character.h"
#include <QDebug>

Investigator::Investigator()
    : m_isInstantiated(false)
{
}

Character *Investigator::instantiate()
{
    if (m_isInstantiated) return NULL;
    Character *c = new Character(this);
    c->instantiateFromInvestigator();
    m_isInstantiated = true;
    return c;
}

bool Investigator::resolveDependecies(GameRegistry *reg) const
{
    bool ok = true;
    if (!m_uniqueAbilityId.isEmpty()) {
        if (reg->findObjectById(m_uniqueAbilityId) == NULL) {
            qWarning() << "Cannot resolve unique ability" << m_uniqueAbilityId << "for character" << id();
            ok = false;
        }
    }

    foreach (QString id, m_fixedPossesionObjectIds) {
        if (reg->findObjectById(id) == NULL) {
            qWarning() << "Cannot resolve fixed possession" << id << "for character" << this->id();
            ok = false;
        }
    }

    return ok;
}
