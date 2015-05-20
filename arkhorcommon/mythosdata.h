#ifndef MYTHOSDATA_H
#define MYTHOSDATA_H

#include <ahglobal.h>
#include "arkhorcommonglobal.h"
#include "serializer.hpp"
#include "fielddata.h"

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS MythosData
        {
        public:
            MythosData();
            virtual ~MythosData() {}

            enum MythosType {
                None,
                Headline,
                Environment,
                Rumor
            };

            enum EnvironmentType {
                Env_None,
            };

            QString id() const { return m_id; }
            MythosType type() const { return m_type; }
            EnvironmentType environmenType() const { return m_envType; }
            QString name() const { return m_name; }
            QString description() const { return m_desc; }
            FieldData::FieldID clueField() const { return m_clueField; }
            FieldData::FieldID gateField() const { return m_gateField; }
            AH::Dimensions whiteMovement() const { return m_moveWhite; }
            AH::Dimensions blackMovement() const { return m_moveBlack; }

        protected:
            MythosType m_type;
            EnvironmentType m_envType;
            QString m_id;
            QString m_name;
            QString m_desc;
            FieldData::FieldID m_clueField;
            FieldData::FieldID m_gateField;
            AH::Dimensions m_moveBlack;
            AH::Dimensions m_moveWhite;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, MythosData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, MythosData::MythosType);
        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, MythosData::EnvironmentType);
    }
}


#endif // MYTHOSDATA_H
