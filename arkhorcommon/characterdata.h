#ifndef CHARACTERDATA_H
#define CHARACTERDATA_H

#include "arkhorcommonglobal.h"
#include <ahglobal.h>
#include <serializer.hpp>
#include <QStringList>

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS CharacterData
        {
        public:
            CharacterData();
            virtual ~CharacterData() {}

            QString id() const { return m_id; }
            QString investigatorId() const { return m_investigatorId; }
            int curStamina() const { return m_curStamina; }
            int curSanity() const { return m_curSanity; }
            int maxStamina() const { return m_maxStamina; }
            int maxSanity() const { return m_maxSanity; }
            int money() const { return m_money; }
            int curFocus() const { return m_curFocus; }
            int movementPoints() const { return m_movementPoints; }
            int clues() const { return m_clues; }
            virtual QList<int> attrSettings() const { return m_attrSettings; }

            QStringList inventoryIds() const { return m_inventoryIds; }
            QStringList monsterMarkerIds() const { return m_monsterMarkerIds; }
            QStringList gateMarkerIds() const { return m_gateMarkerIds; }

            virtual QString fieldId() const { return m_fieldId; }
            bool isDelayed() const { return m_delayed; }
            AH::OtherWorldPhase otherWorldPhase() const { return m_owPhase; }

        protected:
            QString m_id;
            QString m_investigatorId;
            int m_curStamina;
            int m_curSanity;
            int m_maxStamina;
            int m_maxSanity;
            int m_money;
            int m_curFocus;
            int m_movementPoints;
            int m_clues;
            QList<int> m_attrSettings;

            QStringList m_inventoryIds;
            QStringList m_monsterMarkerIds;
            QStringList m_gateMarkerIds;

            QString m_fieldId;
            bool m_delayed;
            AH::OtherWorldPhase m_owPhase;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, CharacterData);
        };
    }
}

#endif // CHARACTERDATA_H
