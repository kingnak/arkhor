#ifndef ATTRIBUTESLIDERDATA_H
#define ATTRIBUTESLIDERDATA_H

#include <ahglobal.h>
#include <serializer.hpp>
#include <arkhorcommonglobal.h>
#include <QList>

namespace AH {
    namespace Common {
        struct ARKHOR_COMMON_EXPORTS AttributeValue
        {
            AH::Attribute attribute;
            quint8 value;

            AttributeValue(AH::Attribute attribute, quint8 value)
                : attribute(attribute), value(value) {}

            bool operator == (const AttributeValue &o) const {
                return attribute == o.attribute && value == o.value;
            }

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, AttributeValue);
        };

        class ARKHOR_COMMON_EXPORTS AttributePair
        {
        public:
            AttributePair(AttributeValue first, AttributeValue second)
                : m_first(first), m_second(second) {}

            const AttributeValue first() const { return m_first; }
            const AttributeValue second() const { return m_second; }

            AttributePair() : m_first(AH::NoAttribute, 0), m_second(AH::NoAttribute, 0) {}

            bool operator == (const AttributePair &o) const {
                return m_first == o.m_first && m_second == o.m_second;
            }

        private:
            AttributeValue m_first;
            AttributeValue m_second;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, AttributePair);
        };

        class ARKHOR_COMMON_EXPORTS AttributeSliderData {
        public:
            QList<AttributePair> availableSettings() const { return m_attributeSettings; }
            qint8 currentSettingPos() const { return m_curSettingPos; }

            const AttributeSliderData *data() const { return this; }

        protected:
            QList<AttributePair> m_attributeSettings;
            qint8 m_curSettingPos;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, AttributeSliderData);
        };

    }
}

#endif // ATTRIBUTESLIDERDATA_H
