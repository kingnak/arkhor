#ifndef ASYNCOBJECTRECEIVER_H
#define ASYNCOBJECTRECEIVER_H

#include <objectdata.h>

struct AsyncObjectReceiver {
    virtual void objectDescribed(const AH::Common::DescribeObjectsData::ObjectDescription &desc) = 0;
    virtual ~AsyncObjectReceiver();
};

#endif // ASYNCOBJECTRECEIVER_H
