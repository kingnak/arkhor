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

            enum AutoSkipData {
                NoAutoSkip,
                AutoSkip,
                AutoChoose
            };
            AutoSkipData autoSkip() const {  return m_autoSkip; }

        protected:
            QString m_id;
            QString m_invId;
            AutoSkipData m_autoSkip;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, PlayerData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, PlayerData::AutoSkipData);
    }
}

#endif // PLAYERDATA_H
