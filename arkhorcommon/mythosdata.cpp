#include "mythosdata.h"

namespace AH { namespace Common {

DEFINE_ENUM_SERIALIZER(MythosData::MythosType)
DEFINE_ENUM_SERIALIZER(MythosData::EnvironmentType)

MythosData::MythosData()
:   m_type(None),
    m_clueField(AH::Common::FieldData::NO_NO_FIELD),
    m_gateField(AH::Common::FieldData::NO_NO_FIELD),
    m_moveBlack(AH::NoDimension),
    m_moveWhite(AH::NoDimension)
{
}

void operator <<(QVariant &data, const MythosData &target)
{
    QVariantMap map;
    map["type"] << target.type();
    map["envType"] << target.environmenType();
    map["id"] << target.id();
    map["name"] << target.name();
    map["desc"] << target.description();
    map["shortDesc"] << target.shortDescription();
    map["clueField"] << target.clueField();
    map["gateField"] << target.gateField();
    map["moveBlack"] << target.blackMovement();
    map["moveWhite"] << target.whiteMovement();
    map["properties"] << target.dynamicProperties();
    map["modifications"] << target.getModificationData();
    data << map;
}

void operator >>(const QVariant &data, MythosData &target)
{
    QVariantMap map;
    data >> map;
    map["type"] >> target.m_type;
    map["envType"] >> target.m_envType;
    map["id"] >> target.m_id;
    map["name"] >> target.m_name;
    map["desc"] >> target.m_desc;
    map["shortDesc"] >> target.m_shortDesc;
    map["clueField"] >> target.m_clueField;
    map["gateField"] >> target.m_gateField;
    map["moveBlack"] >> target.m_moveBlack;
    map["moveWhite"] >> target.m_moveWhite;
    map["properties"] >> target.m_dynProps;
    map["modifications"] >> target.m_modsData;
}
}}
