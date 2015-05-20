#ifndef GAMEOBJECTDATA_H
#define GAMEOBJECTDATA_H

#include "arkhorcommonglobal.h"
#include <serializer.hpp>
#include <ahglobal.h>
#include <QStringList>
//#include <propertymodfier.h>

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS GameObjectData
        {
        public:
            GameObjectData();
            virtual ~GameObjectData() {}

            const GameObjectData *data() const { return this; }

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

            virtual QStringList actionIds() const { return m_actionIds; }
            virtual QStringList optionIds() const { return m_optionIds; }
            //virtual PropertyModifyerList getModifiers();

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
            //PropertyM

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameObjectData);
        };

    }
}

#endif // GAMEOBJECTDATA_H
