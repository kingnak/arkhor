#ifndef PLAYERDATA_H
#define PLAYERDATA_H

#include "arkhorcommonglobal.h"
#include <serializer.hpp>

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS PlayerData
        {
        public:
            PlayerData();
            virtual ~PlayerData() {}

            const PlayerData *data() const { return this; }

            QString id() const { return m_id; }
            QString investigatorId() const { return m_invId; }

        protected:
            QString m_id;
            QString m_invId;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, PlayerData);
        };
    }
}

#endif // PLAYERDATA_H
