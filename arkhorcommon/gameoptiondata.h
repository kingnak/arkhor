#ifndef GAMEOPTIONDATA_H
#define GAMEOPTIONDATA_H

#include "arkhorcommonglobal.h"
#include <serializer.hpp>
#include <ahglobal.h>
#include <cost.h>

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS GameOptionData
        {
        public:
            GameOptionData();
            virtual ~GameOptionData();

            QString id() const { return m_id; }
            virtual QString name() const { return m_name; }
            virtual QString description() const { return m_description; }
            QString sourceId() const { return m_sourceId; }
            virtual AH::GamePhases phases() const { return m_phases; }
            virtual QString actionId() const { return m_actionId; }

            virtual Cost costs() const { return m_costs; }

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

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameOptionData);
        };

    }
}


#endif // GAMEOPTIONDATA_H
