#ifndef GAMESETTINGDATA_H
#define GAMESETTINGDATA_H

#include "arkhorcommonglobal.h"
#include "ahglobal.h"
#include "serializer.hpp"

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS GameSettingData
        {
        public:
            GameSettingData(int tl = 0, QString aoId = QString::null, QString rId = QString::null, QString eId = QString::null);
            virtual ~GameSettingData() {}

            int terrorLevel() const { return m_terror; }
            QString ancientOneId() const { return m_ancientOneId; }
            QString rumorId() const { return m_rumorId; }
            QString environmentId() const { return m_environmentId; }

            void setTerrorLevel(int level) { m_terror = level; }
            void setAncientOneId(QString a) { m_ancientOneId = a; }
            void setRumorId(QString r) { m_rumorId = r; }
            void setEnvironmentId(QString e) { m_environmentId = e; }

        protected:
            int m_terror;
            QString m_ancientOneId;
            QString m_rumorId;
            QString m_environmentId;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GameSettingData);
        };
    }
}


#endif // GAMESETTINGDATA_H
