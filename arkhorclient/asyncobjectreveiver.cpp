#include "asyncobjectreceiver.h"
#include "objectregistry.h"

AsyncObjectReceiver::~AsyncObjectReceiver()
{
   ObjectRegistry::instance()->unsubscribe(this);
}
