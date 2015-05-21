#ifndef MYTHOSCARD_H
#define MYTHOSCARD_H

#include <mythosdata.h>
#include "propertymodifier.h"

class GameOption;

class MythosCard : public AH::Common::MythosData
{
public:
    MythosCard();

    void setId(QString id) { m_id = id; }

    virtual bool resolveDependencies() = 0;
    virtual void resolveDynamicAttributes() = 0;

    // Headlines
    virtual void executeHeadline() = 0;

    // Environment
    virtual PropertyModificationList getModifications() const = 0;
    virtual QList<AH::Common::PropertyModificationData> getModificationData() const;

    // Rumors
    virtual void onMythos() = 0;
    virtual void onPass() = 0;
    virtual void onFail() = 0;
    virtual void setupRumor() = 0;
    virtual void teardownRumor() = 0;
    virtual GameOption *rumorFieldOption() = 0;
    virtual AH::Common::FieldData::FieldID rumorFieldId() = 0;

    virtual void pass();
    virtual void fail();

    /*
    void resolve();

    virtual void deactivate() = 0;
    virtual PropertyModificationList environmentModifictions() = 0;
    virtual bool checkRumorState() = 0;

protected:
    virtual void executeHeadline() = 0;
    */

private:
    void cleanup();
};

#endif // MYTHOSCARD_H
