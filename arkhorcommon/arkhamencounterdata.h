#ifndef ARKHAMENCOUNTERDATA_H
#define ARKHAMENCOUNTERDATA_H

#include "arkhorcommonglobal.h"
#include "serializer.hpp"
#include "fielddata.h"
#include <QStringList>

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS ArkhamEncounterData
        {
        public:
            ArkhamEncounterData();
            virtual ~ArkhamEncounterData() {}

            FieldData::FieldID fieldId() const { return m_fieldId; }
            QStringList optionIds() const { return m_optionIds; }
            QString description() const { return m_description; }

        protected:
            FieldData::FieldID m_fieldId;
            QStringList m_optionIds;
            QString m_description;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, ArkhamEncounterData);
        };

    }
}


#endif // ARKHAMENCOUNTERDATA_H
