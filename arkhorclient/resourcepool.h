#ifndef RESOURCEPOOL_H
#define RESOURCEPOOL_H

#include <QPixmap>
#include <QFont>
#include <QString>
#include <QMap>
#include <ahglobal.h>
#include <fielddata.h>

class ResourcePool
{
public:
    static ResourcePool *instance();

    bool addDirectory(QString dir);
    bool addZip(QString zip);

    QPixmap loadMonster(QString id);
    QPixmap loadCharacterFigure(QString id);
    QFont loadMainFont();
    QPixmap loadDimensionSymbol(AH::Dimension dim);
    QPixmap loadOtherWorldGate(AH::Common::FieldData::FieldID id);
    QPixmap loadObjectImage(QString id, AH::GameObjectType type);

private:
    struct ResourceDef {
        enum Type {
            None, Zip, File
        } type;
        QString base;
        QString rel;

        ResourceDef() : type(None) {}
        ResourceDef(QString f) : type(File), base(f) {}
        ResourceDef(QString f, QString r) : type(Zip), base(f), rel(r) {}
    };

private:
    bool addEntry(QString e, ResourceDef d);
    QPair<QString, QString> getIdFromEntry(QString e);
    QByteArray intLoadResource(QString id, QString sub = QString::null);
    QPixmap intLoadPixmap(QString id, QString sub = QString::null);

    QMap<QString, QPixmap> m_pixmapCache;
    QMap<QString, QMap<QString, ResourceDef> > m_resPaths;
    QMap<QString, QPair<QString, QString> > m_monsterIdCache;
    bool m_mainFontLoaded;

private:
    ResourcePool();
    Q_DISABLE_COPY(ResourcePool)
};

#endif // RESOURCEPOOL_H
