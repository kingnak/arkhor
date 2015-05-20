#ifndef ARKHAMENCOUNTERDATA_H
#define ARKHAMENCOUNTERDATA_H

#include "arkhorcommonglobal.h"
#include "serializer.hpp"
#include "fielddata.h"
#include <QStringList>
#include "gameoptiondata.h"

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS ArkhamEncounterData
        {
        public:
            ArkhamEncounterData();
            virtual ~ArkhamEncounterData() {}

            virtual ArkhamEncounterData *data() { return this; }

            FieldData::FieldID fieldId() const { return m_fieldId; }
            //virtual QStringList optionIds() const { return m_optionIds; }
            virtual QList<GameOptionData> optionData() const { return m_optionData; }
            QString description() const { return m_description; }

        protected:
            FieldData::FieldID m_fieldId;
            //QStringList m_optionIds;
            QString m_description;
            QList<GameOptionData> m_optionData;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, ArkhamEncounterData);
        };

    }
}


#endif // ARKHAMENCOUNTERDATA_H
