#include "resourcepool.h"
#include <QFontDatabase>
#include <quazip/quazip.h>
#include <quazip/quazipfile.h>
#include <QImageReader>
#include <QBuffer>
#include <QFileInfoList>
#include <QDir>
#include <QDebug>

ResourcePool *ResourcePool::instance()
{
    static ResourcePool inst_;
    return &inst_;
}

bool ResourcePool::addDirectory(QString dir)
{
    QDir base(dir);
    QFileInfoList lst = base.entryInfoList(QDir::AllDirs | QDir::NoDotAndDotDot | QDir::Files, QDir::Name);
    foreach (QFileInfo fi, lst) {
        if (fi.isDir()) {
            if (!addDirectory(fi.absoluteFilePath()))
                return false;
        } else {
            addEntry(QDir::fromNativeSeparators(fi.fileName()), ResourceDef(fi.absoluteFilePath()));
        }
    }

    return true;
}

bool ResourcePool::addZip(QString zip)
{
    QuaZip z(zip);
    if (!z.open(QuaZip::mdUnzip)) return false;

    QList<QuaZipFileInfo> entries = z.getFileInfoList();
    foreach (QuaZipFileInfo e, entries) {
        if (e.uncompressedSize > 0) {
            addEntry(e.name, ResourceDef(zip, e.name));
        }
    }

    return true;
}

QPixmap ResourcePool::loadMonster(QString id)
{
    if (id.isEmpty()) return QPixmap();
    id = id.toLower();
    QString sub = "";
    QString rid = id;
    if (m_monsterIdCache.contains(id)) {
        rid = m_monsterIdCache[id].first;
        sub = m_monsterIdCache[id].second;
    } else {
        QRegExp rx("^(.*):(\\d+)$");
        if (rx.indexIn(id) >= 0) {
            rid = rx.cap(1);
            // This converts instance counter to a number in
            // multiple resources.
            // TODO: Random instead?
            int n = rx.cap(2).toInt();
            int ct = m_resPaths.value(rid).count();
            if (ct > 1) {
                n %= ct;
                sub = QString::number(n+1);
            }
        }
        m_monsterIdCache[id] = qMakePair(rid, sub);
    }

    QPixmap ret = intLoadPixmap(rid, sub);
    if (ret.isNull()) return QPixmap(":/core/images/unknown_monster");
    return ret;
}

QPixmap ResourcePool::loadCharacterFigure(QString id)
{
    QPixmap ret = intLoadPixmap(id, "figure");
    if (ret.isNull()) return QPixmap(":/core/images/unknown_character");
    return ret;
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

QPixmap ResourcePool::loadOtherWorldGate(AH::Common::FieldData::FieldID id)
{
    switch (id) {
    case AH::Common::FieldData::OW_Abyss: return QPixmap(":/core/images/gate_abyss");
    case AH::Common::FieldData::OW_CityOfGreatRace: return QPixmap(":/core/images/gate_city_of_great_race");
    case AH::Common::FieldData::OW_Yuggoth: return QPixmap(":/core/images/gate_yuggoth");
    case AH::Common::FieldData::OW_Celeano: return QPixmap(":/core/images/gate_celeano");
    case AH::Common::FieldData::OW_TheDreamlands: return QPixmap(":/core/images/gate_dreamlands");
    case AH::Common::FieldData::OW_PlateauOfLeng: return QPixmap(":/core/images/gate_plateau_of_leng");
    case AH::Common::FieldData::OW_R_lyeh: return QPixmap(":/core/images/gate_r_lyeh");
    case AH::Common::FieldData::OW_AnotherDimension: return QPixmap(":/core/images/gate_another_dimension");
    default:
        return QPixmap();
    }
}

QPixmap ResourcePool::loadObjectImage(QString id, AH::GameObjectType type)
{
    // TODO: id specific images
    switch (type) {
    case AH::Obj_CommonItem: return QPixmap(":/core/images/common_item_back");
    case AH::Obj_UniqueItem: return QPixmap(":/core/images/unique_item_back");
    case AH::Obj_Spell: return QPixmap(":/core/images/spell_back");
    case AH::Obj_Skill: return QPixmap(":/core/images/skill_back");
    case AH::Obj_Ally: return QPixmap(":/core/images/ally_back");
        // TODO: Special?
    //case AH::Obj_Special: return QPixmap(":/core/images/Special_back");
    case AH::Obj_Blessing_Curse:
        // TODO: Better
        if (id.toUpper().contains("BLESSING"))
            return QPixmap(":/core/images/blessing_card");
        else if (id.toUpper().contains("CURSE"))
            return QPixmap(":/core/images/curse_card");
    case AH::NoObject:
        break;
    }

    return QPixmap();
}

bool ResourcePool::addEntry(QString e, ResourcePool::ResourceDef d)
{
    QPair<QString, QString> data = getIdFromEntry(e);
    QString id = data.first;
    QString sub = data.second;
    if (id.isNull()) return false;
    if (m_resPaths.contains(id) && m_resPaths[id].contains(sub)) {
        qWarning() << "Overriding previous resource for id" << id << ", Subitem" << sub;
    }

    m_resPaths[id][sub] = d;
    return true;
}

QPair<QString, QString> ResourcePool::getIdFromEntry(QString e)
{
    int ls = e.lastIndexOf('/');
    e = e.mid(ls+1);

    QRegExp rx("^([^-]+)(?:-([^-\\.]+))?\\.(.*)$");
    if (rx.exactMatch(e)) {
        QString id = rx.cap(1).toLower();
        QString subItem = rx.cap(2).toLower();
        QString ext = rx.cap(3).toLower();

        qDebug() << "Id:" << id << "Sub:" << subItem << "Type:" << ext;
        return qMakePair(id, subItem);
    }
    return qMakePair(QString(), QString());
}

ResourcePool::ResourcePool()
    : m_mainFontLoaded(false)
{

}

QByteArray ResourcePool::intLoadResource(QString id, QString sub)
{
    id = id.toLower();
    sub = sub.toLower();
    
    if (id.isEmpty() || !m_resPaths.contains(id)) {
        return QByteArray();
    }

    ResourceDef rd = m_resPaths[id].value(sub);
    QIODevice *dev = NULL;
    if (rd.type == ResourceDef::Zip) {
        dev = new QuaZipFile(rd.base, rd.rel);
    } else if (rd.type == ResourceDef::File) {
        dev = new QFile(rd.base);
    } else {
        return QByteArray();
    }

    if (!dev->open(QuaZipFile::ReadOnly)) {
        delete dev;
        return QByteArray();
    }

    QByteArray d = dev->readAll();
    delete dev;
    return d;
}

QPixmap ResourcePool::intLoadPixmap(QString id, QString sub)
{
    QString key = id + "-" + sub;
    if (m_pixmapCache.contains(key)) return m_pixmapCache[key];

    QByteArray ba = intLoadResource(id, sub);
    if (ba.isEmpty()) return QPixmap();

    QBuffer buf(&ba);
    buf.open(QIODevice::ReadOnly);
    QImageReader r(&buf);
    QImage img;
    QPixmap ret;
    if (r.read(&img)) {
        ret.convertFromImage(img);
    } else {
        ret = QPixmap();
    }
    m_pixmapCache[key] = ret;
    return ret;
}
