#ifndef CHOICEDATA_H
#define CHOICEDATA_H

#include <ahglobal.h>
#include "arkhorcommonglobal.h"
#include "serializer.hpp"
#include <QStringList>
#include "cost.h"

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS ChoiceData
        {
        public:
            enum ChoiceType {
                None,
                ChooseObjects,
                ChoosePayment,
                ChooseString,
            };
            struct OptionData {
                QString id, name, description;
                OptionData(QString id = QString::null, QString name = QString::null, QString desc = QString::null)
                    : id(id), name(name), description(desc) {}
                DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, OptionData);
            };

            ChoiceData(ChoiceType type = None, QString desc = QString::null)
                : m_type(type), m_desc(desc), m_canCancel(false), m_min(0), m_max(0) {}

            ChoiceType type() const { return m_type; }
            QString description() const { return m_desc; }
            QString sourceId() const { return m_sourceId; }
            bool canCancel() const { return m_canCancel; }
            int getMin() const { return m_min; }
            int getMax() const { return m_max; }
            QStringList getObjectIds() const { return m_objectIds; }
            Cost getCosts() const { return m_costs; }
            QList<OptionData> getStrings() const { return m_strings; }

            void setType(ChoiceType type) { m_type = type; }
            void setDescription(const QString &desc) { m_desc = desc; }
            void setSourceId(const QString &sourceId) { m_sourceId = sourceId; }
            void setCanCancel(bool canCancel) { m_canCancel = canCancel; }

            void setSelectObjects(QStringList objectIds, int min = 0, int max = 1);
            void setSelectPayment(Cost c);

            void setSelectStrings(QList<OptionData> options);

        private:
            ChoiceType m_type;
            QString m_desc;
            QString m_sourceId;
            bool m_canCancel;

            // Objects
            int m_min;
            int m_max;
            QStringList m_objectIds;

            // Cost
            Cost m_costs;

            // Strings
            QList<OptionData> m_strings;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, ChoiceData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, ChoiceData::ChoiceType);

        typedef QVariant ChoiceResponseData;
    }
}


#endif // CHOICEDATA_H
