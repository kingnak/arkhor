#ifndef GAMEACTIONDATA_H
#define GAMEACTIONDATA_H

#include "arkhorcommonglobal.h"
#include <serializer.hpp>
#include <ahglobal.h>

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS GameActionData
        {
        public:
            GameActionData();
            virtual ~GameActionData() = default;

            virtual QString id() const { return m_id; }
            virtual QString name() const { return m_name; }
            virtual QString description() const { return m_description; }
            virtual QString sourceId() const { return m_sourceId; }
            virtual AH::GamePhases phases() const { return m_phases; }

        protected:
            QString m_id;
            QString m_name;
            QString m_description;
            QString m_sourceId;
            AH::GamePhases m_phases;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameActionData);
        };

    }
}


#endif // GAMEACTIONDATA_H
