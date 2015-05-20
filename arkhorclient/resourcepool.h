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

private:
    QString getIdFromEntry(QString e);
    QByteArray intLoadResource(QString id);
    QPixmap intLoadPixmap(QString id);

private:
    struct ResourceDef {
        //QString id;
        enum Type {
            None, Zip, File
        } type;
        QString base;
        QString rel;

        ResourceDef() : type(None) {}
        ResourceDef(QString f) : type(File), base(f) {}
        ResourceDef(QString f, QString r) : type(Zip), base(f), rel(r) {}
    };

    QMap<QString, QPixmap> m_pixmapCache;
    QMap<QString, ResourceDef> m_resPaths;
    bool m_mainFontLoaded;

private:
    ResourcePool();
    Q_DISABLE_COPY(ResourcePool)
};

#endif // RESOURCEPOOL_H
