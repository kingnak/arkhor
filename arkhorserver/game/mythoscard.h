#ifndef MYTHOSCARD_H
#define MYTHOSCARD_H

#include <mythosdata.h>
#include "propertymodifier.h"
#include "monstermodifier.h"

class GameOption;

class MythosCard : public AH::Common::MythosData, public PropertyModifier, public MonsterModifier
{
public:
    MythosCard();

    void setId(QString id) { m_id = id; }

    QString modifierId() const override { return m_id; }

    virtual bool resolveDependencies() = 0;
    virtual void resolveDynamicAttributes() = 0;

    // Headlines
    virtual void executeHeadline() = 0;

    // Environment
    PropertyModificationList getModifications() const override = 0;
    QList<AH::Common::PropertyModificationData> getModificationData() const override;
    virtual GameOption *environmentFieldOption() = 0;
    virtual AH::Common::FieldData::FieldID environmentFieldId() = 0;
    virtual void activateEnvironment() = 0;
    virtual void onEndMovement() = 0;
    virtual bool onDefeatMonster(Character *byCharacter, Monster *m) = 0;
    virtual bool onAppearMonster(GameField *f, Monster *m) = 0;

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

    void cleanup();

    /*
    void resolve();

    virtual void deactivate() = 0;
    virtual PropertyModificationList environmentModifictions() = 0;
    virtual bool checkRumorState() = 0;

protected:
    virtual void executeHeadline() = 0;
    */

protected:
    const PropertyModifier *getMonsterMovementModifier() const override { return this; }
};

#endif // MYTHOSCARD_H
