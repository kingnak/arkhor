#ifndef MYTHOSCARD_H
#define MYTHOSCARD_H

#include <mythosdata.h>
#include "propertymodifier.h"

class MythosCard : public AH::Common::MythosData
{
public:
    MythosCard();

    void setId(QString id) { m_id = id; }

    /*
    void resolve();

    virtual void deactivate() = 0;
    virtual PropertyModificationList environmentModifictions() = 0;
    virtual bool checkRumorState() = 0;

protected:
    virtual void executeHeadline() = 0;
    */

};

#endif // MYTHOSCARD_H
