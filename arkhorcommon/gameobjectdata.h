#ifndef GAMEOBJECTDATA_H
#define GAMEOBJECTDATA_H

#include "arkhorcommonglobal.h"
#include <serializer.hpp>
#include <ahglobal.h>
#include <QStringList>
#include "propertyvaluedata.h"
#include "dynamicpropertydata.h"

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS GameObjectData
        {
        public:
            GameObjectData();
            virtual ~GameObjectData() {}

            enum ObjectAttribute {
                None,
                CannotBeLost = 0x0001,
                DiscardAfterAttack = 0x0002,

                ObjectAttribute_Max_Value_Sentinel
            };
            Q_DECLARE_FLAGS(ObjectAttributes, ObjectAttribute)

            virtual GameObjectData *data() { return this; }

            QString id() const { return m_id; }
            QString typeId() const { return m_tid; }
            QString name() const { return m_name; }
            QString description() const { return m_description; }
            AH::GameObjectType type() const { return m_type; }
            virtual int handCount() const { return m_hands; }
            virtual int price() const { return m_price; }

            virtual bool isExhausted() const {
                if (m_bExhaustable) {
                    return m_bIsExhausted;
                }
                return false;
            }

            virtual bool isEquipped() const { return m_isEquipped; }

            virtual int castCost() const { return m_castCost; }
            virtual int castAdjustment() const { return m_castAdjustment; }
            virtual int castTarget() const { return m_castTarget; }

            virtual QStringList actionIds() const { return m_actionIds; }
            virtual QStringList optionIds() const { return m_optionIds; }
            virtual QList<PropertyModificationData> getModificationData() const { return m_modsData; }

            virtual ObjectAttributes getAttributes() const { return m_attrs; }

            virtual QList<DynamicPropertyData> dynamicProperties() const { return m_dynProps; }

        protected:
            QString m_id;
            QString m_tid;
            QString m_name;
            QString m_description;
            AH::GameObjectType m_type;
            int m_hands;
            int m_price;
            bool m_bExhaustable;
            bool m_bIsExhausted;
            QStringList m_actionIds;
            QStringList m_optionIds;
            bool m_isEquipped;
            ObjectAttributes m_attrs;

            QList<PropertyModificationData> m_modsData;

            QList<DynamicPropertyData> m_dynProps;

            // For Spells
            int m_castCost;
            int m_castAdjustment;
            int m_castTarget;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameObjectData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, GameObjectData::ObjectAttribute);
        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, GameObjectData::ObjectAttributes);

    }
}

#endif // GAMEOBJECTDATA_H
