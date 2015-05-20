#ifndef GAMEFIELDDATA_H
#define GAMEFIELDDATA_H

#include "arkhorcommonglobal.h"
#include <serializer.hpp>
#include <QStringList>
#include "fielddata.h"

namespace AH {
    namespace Common {
        class ARKHOR_COMMON_EXPORTS GameFieldData
        {
        public:
            GameFieldData();
            virtual ~GameFieldData() {}

            GameFieldData *data() { return this; }

            FieldData::FieldID id() const { return m_id; }
            QString name() const { return m_name; }
            FieldData::FieldType type() const { return m_type; }

            bool isSealed() const { return m_sealed; }

            virtual QStringList characterIds() const { return m_characterIds; }
            virtual QStringList monsterIds() const { return m_monsterIds; }
            virtual QString gateId() const { return m_gateId; }
            virtual QList<int> neighbourIds() const { return m_neighbourIds; }

            int clueAmount() const { return m_clues; }

        protected:
            FieldData::FieldID m_id;
            QString m_name;
            FieldData::FieldType m_type;
            int m_clues;
            bool m_sealed;

            QStringList m_characterIds;
            QStringList m_monsterIds;
            QString m_gateId;
            QList<int> m_neighbourIds;


            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameFieldData);
        };
    }
}

Q_DECLARE_METATYPE(AH::Common::GameFieldData)

#endif // GAMEFIELDDATA_H
