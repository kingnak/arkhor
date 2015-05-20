#ifndef ANCIENTONEDATA_H
#define ANCIENTONEDATA_H

#include <ahglobal.h>
#include "arkhorcommonglobal.h"
#include "serializer.hpp"
#include "monsterdata.h"

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS AncientOneData
        {
        public:
            AncientOneData();
            virtual ~AncientOneData() {}

            virtual AncientOneData *data() { return this; }

            QString id() const { return m_id; }

            QString name() const { return m_name; }
            QString worshippersText() const { return m_worshippersText; }
            QString powerText() const { return m_powerText; }
            QString battleStartText() const { return m_battleStartText; }
            QString attackText() const { return m_attackText; }
            virtual MonsterData::MonsterAttributes defenses() const { return m_defenses; }
            int doomTrack() const { return m_doomTrack; }
            virtual int combatAdjustment() const { return m_combatAdjustment; }

        protected:
            QString m_id;
            QString m_name;
            QString m_worshippersText;
            QString m_powerText;
            QString m_battleStartText;
            QString m_attackText;
            MonsterData::MonsterAttributes m_defenses;
            int m_doomTrack;
            int m_combatAdjustment;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, AncientOneData);
        };

    }
}

#endif // ANCIENTONEDATA_H
