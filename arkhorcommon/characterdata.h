#ifndef CHARACTERDATA_H
#define CHARACTERDATA_H

#include "arkhorcommonglobal.h"
#include <ahglobal.h>
#include <fielddata.h>
#include <serializer.hpp>
#include <QStringList>
#include <investigatordata.h>

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS CharacterData
        {
        public:
            CharacterData();
            virtual ~CharacterData() {}

            virtual CharacterData *data() { return this; }

            QString id() const { return m_id; }
            QString investigatorId() const { return m_investigatorId; }
            virtual InvestigatorData investigatorData() const { return m_investigatorData; }
            int curStamina() const { return m_curStamina; }
            int curSanity() const { return m_curSanity; }
            virtual int maxStamina() const { return m_maxStamina; }
            virtual int maxSanity() const { return m_maxSanity; }
            int money() const { return m_money; }
            int curFocus() const { return m_curFocus; }
            int movementPoints() const { return m_movementPoints; }
            int clues() const { return m_clues; }
            virtual QList<int> attrSettings() const { return m_attrSettings; }

            QStringList inventoryIds() const { return m_inventoryIds; }
            QStringList monsterMarkerIds() const { return m_monsterMarkerIds; }
            QStringList gateMarkerIds() const { return m_gateMarkerIds; }
            int getMonsterToughness() const { return m_monsterToughness; }

            virtual FieldData::FieldID fieldId() const { return m_fieldId; }
            bool isDelayed() const { return m_delayed; }
            bool isSetOut() const { return m_isSetOut; }

            AH::OtherWorldPhase otherWorldPhase() const { return m_owPhase; }

        protected:
            QString m_id;
            QString m_investigatorId;
            InvestigatorData m_investigatorData;
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
            int m_monsterToughness;

            FieldData::FieldID m_fieldId;
            bool m_delayed;
            bool m_isSetOut;
            AH::OtherWorldPhase m_owPhase;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, CharacterData);
        };
    }
}

#endif // CHARACTERDATA_H
