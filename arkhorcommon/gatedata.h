#ifndef GATEDATA_H
#define GATEDATA_H

#include "arkhorcommonglobal.h"
#include "serializer.hpp"
#include <ahglobal.h>
#include "fielddata.h"

namespace AH {
    namespace Common {

        class ARKHOR_COMMON_EXPORTS GateData
        {
        public:
            GateData();
            virtual ~GateData() {}

            virtual GateData *data() { return this; }

            virtual QString id() const { return m_id; }
            virtual bool isOpen() const { return m_open; }
            virtual FieldData::FieldID source() const { return m_source; }
            virtual FieldData::FieldID destination() const { return m_dest; }
            virtual AH::Dimensions dimensions() const { return m_dims; }
            virtual int closeAdjustment() const { return m_adjustment; }

        protected:
            public:
            QString m_id;
            bool m_open;
            FieldData::FieldID m_source;
            FieldData::FieldID m_dest;
            AH::Dimensions m_dims;
            int m_adjustment;

            DECLARE_SERIALIZABLE_EXPORT(ARKHOR_COMMON_EXPORTS, GateData);
        };

    }
}

#endif // GATEDATA_H
