#ifndef GAMEOPTIONDATA_H
#define GAMEOPTIONDATA_H

#include "arkhorcommonglobal.h"
#include <serializer.hpp>
#include <ahglobal.h>
#include <cost.h>
#include <propertyvaluedata.h>

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS GameOptionData
        {
        public:
            GameOptionData();
            virtual ~GameOptionData() = default;

            virtual GameOptionData *data() { return this; }

            virtual QString id() const { return m_id; }
            virtual QString name() const { return m_name; }
            virtual QString description() const { return m_description; }
            virtual QString sourceId() const { return m_sourceId; }
            virtual AH::GamePhases phases() const { return m_phases; }
            virtual QString actionId() const { return m_actionId; }

            virtual Cost costs() const { return m_costs; }
            virtual bool canPay() const { return m_canPay; }
            virtual bool isAvailable() const { return m_isAvailable; }
            virtual ModifiedPropertyValueData baseProperty() const { return m_baseProperty; }

            virtual AH::ChooseType chooseType() const { return m_chooseType; }
            virtual AH::ContinueType continueType() const { return m_continueType; }

        protected:
            QString m_id;
            QString m_name;
            QString m_description;
            QString m_sourceId;
            QString m_actionId;
            AH::GamePhases m_phases;
            Cost m_costs;
            ChooseType m_chooseType;
            ContinueType m_continueType;
            bool m_isAvailable;
            bool m_canPay;
            ModifiedPropertyValueData m_baseProperty;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameOptionData);
        };

    }
}


#endif // GAMEOPTIONDATA_H
