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

    static const QColor StaminaColor;
    static const QColor SanityColor;

    bool addDirectory(const QString &dir);
    bool addZip(const QString &zip);

    QPixmap loadMonster(QString id);
    QPixmap loadCharacterFigure(const QString &id);
    QFont loadMainFont();
    QPixmap loadDimensionSymbol(AH::Dimension dim);
    QPixmap loadOtherWorldGate(AH::Common::FieldData::FieldID id);
    QPixmap loadObjectImage(const QString &id, AH::GameObjectType type);
    QPixmap loadAncientOne(const QString &id);

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
    bool addEntry(const QString &e, const ResourceDef &d);
    QPair<QString, QString> getIdFromEntry(QString e);
    QByteArray intLoadResource(QString id, QString sub = QString::null);
    QPixmap intLoadPixmap(const QString &id, const QString &sub = QString::null);

    QMap<QString, QPixmap> m_pixmapCache;
    QMap<QString, QMap<QString, ResourceDef> > m_resPaths;
    QMap<QString, QPair<QString, QString> > m_monsterIdCache;
    bool m_mainFontLoaded;

private:
    ResourcePool();
    Q_DISABLE_COPY(ResourcePool)
};

#endif // RESOURCEPOOL_H
