#ifndef INVESTIGATORDATA_H
#define INVESTIGATORDATA_H

#include "arkhorcommonglobal.h"
#include "ahglobal.h"
#include "fielddata.h"
#include <QStringList>
#include "serializer.hpp"

namespace AH {
    namespace Common
    {
        class ARKHOR_COMMON_EXPORTS InvestigatorData
        {
        public:
            InvestigatorData();
            virtual ~InvestigatorData() {}

            struct AttributeValuePair {
                int first;
                int second;
                AttributeValuePair(int f = 0, int s = 0)
                    : first(f), second(s) {}
            };

            struct ObjectTypeCount {
                AH::GameObjectType type;
                int amount;
                ObjectTypeCount(AH::GameObjectType t = AH::NoObject, int a = 0)
                    : type(t), amount(a) {}
            };

            const InvestigatorData *data() const { return this; }

            QString id() const { return m_id; }
            QString name() const { return m_name; }
            QString occupation() const { return m_occupation; }
            FieldData::FieldID startFieldId() const { return m_homeFieldId; }
            int sanity() const { return m_sanity; }
            int stamina() const { return m_stamina; }
            int money() const { return m_money; }
            int focus() const { return m_focus; }
            QList<AttributeValuePair> attrSpeedSneak() const { return m_attrSpeedSneak; }
            QList<AttributeValuePair> attrFightWill() const { return m_attrFightWill; }
            QList<AttributeValuePair> attrLoreLuck() const { return m_attrLoreLuck; }
            QString uniqueAbilityId() const { return m_uniqueAbilityId; }
            QStringList fixedPossesionObjectIds() const { return m_fixedPossesionObjectIds; }
            QList<ObjectTypeCount> randomPossesions() const { return m_randomPossessions; }

        protected:
            QString m_id;
            QString m_name;
            QString m_occupation;
            FieldData::FieldID m_homeFieldId;
            int m_sanity;
            int m_stamina;
            int m_money;
            int m_focus;
            QList<AttributeValuePair> m_attrSpeedSneak;
            QList<AttributeValuePair> m_attrFightWill;
            QList<AttributeValuePair> m_attrLoreLuck;

            QString m_uniqueAbilityId;
            QStringList m_fixedPossesionObjectIds;
            QList<ObjectTypeCount> m_randomPossessions;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, InvestigatorData);
            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, InvestigatorData::AttributeValuePair);
            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, InvestigatorData::ObjectTypeCount);
        };

        //DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, Inv);
    }
}
/*
Q_DECLARE_METATYPE(AH::Common::InvestigatorData::AttributeValuePair);
Q_DECLARE_METATYPE(AH::Common::InvestigatorData::ObjectTypeCount);
*/
#endif // INVESTIGATORDATA_H
