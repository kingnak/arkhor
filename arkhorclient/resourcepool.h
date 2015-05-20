#ifndef RESOURCEPOOL_H
#define RESOURCEPOOL_H

#include <QPixmap>
#include <QFont>
#include <QString>
#include <ahglobal.h>

class ResourcePool
{
public:
    static ResourcePool *instance();

    QPixmap loadMonster(QString id);
    QFont loadMainFont();
    QPixmap loadDimensionSymbol(AH::Dimension dim);

private:
    ResourcePool();
    Q_DISABLE_COPY(ResourcePool)

    bool m_mainFontLoaded;
};

#endif // RESOURCEPOOL_H
