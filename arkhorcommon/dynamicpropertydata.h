#ifndef DYNAMICPROPERTYDATA_H
#define DYNAMICPROPERTYDATA_H

#include "arkhorcommonglobal.h"
#include "serializer.hpp"

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS DynamicPropertyData
        {
        public:
            DynamicPropertyData(QString name = QString::null) : m_name(name) {}

            QString name() const { return m_name; }
            QVariant value() const { return m_val; }
            virtual QString display() const { return m_display; }

            void setValue(QVariant val) { m_val = val; }
            void setDisplay(QString disp) { m_display = disp; }

        protected:
            QString m_name;
            QVariant m_val;
            QString m_display;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, DynamicPropertyData);
        };
    }
}


#endif // DYNAMICPROPERTYDATA_H
