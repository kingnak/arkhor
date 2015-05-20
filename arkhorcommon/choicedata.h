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
                ChoosePayment
            };

            ChoiceData(ChoiceType type = None, QString desc = QString::null)
                : m_type(type), m_desc(desc), m_min(0), m_max(0) {}

            ChoiceType type() const { return m_type; }
            QString description() const { return m_desc; }
            int getMin() const { return m_min; }
            int getMax() const { return m_max; }
            QStringList getObjectIds() const { return m_objectIds; }
            Cost getCosts() const { return m_costs; }

            void setType(ChoiceType type) { m_type = type; }
            void setDescription(const QString &desc) { m_desc = desc; }

            void setSelectObjects(QStringList objectIds, int min = 0, int max = 1);
            void setSelectPayment(Cost c);

        private:
            ChoiceType m_type;
            QString m_desc;

            // Objects
            int m_min;
            int m_max;
            QStringList m_objectIds;

            // Cost
            Cost m_costs;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, ChoiceData);
        };

        DECLARE_ENUM_SERIALIZER_EXPORT(ARKHOR_COMMON_EXPORTS, ChoiceData::ChoiceType);

        typedef QVariant ChoiceResponseData;
    }
}


#endif // CHOICEDATA_H
