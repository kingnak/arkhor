#include "resourcepool.h"
#include <QFontDatabase>

ResourcePool *ResourcePool::instance()
{
    static ResourcePool inst_;
    return &inst_;
}

QPixmap ResourcePool::loadMonster(QString id)
{
    if (id == "") return QPixmap(":/core/images/unknown_monster");
    return QPixmap(":/test/client_resources/test/Byakhee.png");
}

QFont ResourcePool::loadMainFont()
{
    if (!m_mainFontLoaded) {
        m_mainFontLoaded = true;
        QFontDatabase::addApplicationFont(":/core/data/fontCarbon");
    }
    return QFont("CarbonType");
}

QPixmap ResourcePool::loadDimensionSymbol(AH::Dimension dim)
{
    switch (dim) {
    case AH::Dim_Square: return QPixmap(":/core/images/dim_square");
    case AH::Dim_Circle: return QPixmap(":/core/images/dim_circle");
    case AH::Dim_Moon: return QPixmap(":/core/images/dim_moon");
    case AH::Dim_Slash: return QPixmap(":/core/images/dim_slash");
    case AH::Dim_Star: return QPixmap(":/core/images/dim_star");
    case AH::Dim_Diamond: return QPixmap(":/core/images/dim_diamond");
    case AH::Dim_Hexagon: return QPixmap(":/core/images/dim_hexagon");
    case AH::Dim_Plus: return QPixmap(":/core/images/dim_plus");
    case AH::Dim_Triangle: return QPixmap(":/core/images/dim_triangle");
    default:
        return QPixmap();
    }
}

ResourcePool::ResourcePool()
    : m_mainFontLoaded(false)
{

}
