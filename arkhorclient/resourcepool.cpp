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
            QString id = getIdFromEntry(QDir::fromNativeSeparators(fi.absoluteFilePath()));
            if (id.isNull()) continue;
            if (m_resPaths.contains(id)) {
                qWarning() << "Overriding previous resource for id" << id;
            }
            m_resPaths[id] = ResourceDef(fi.absoluteFilePath());
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
            QString id = getIdFromEntry(e.name);
            if (id.isNull()) continue;
            if (m_resPaths.contains(id)) {
                qWarning() << "Overriding previous resource for id" << id;
            }
            m_resPaths[id] = ResourceDef(zip, e.name);
        }
    }

    return true;
}

QPixmap ResourcePool::loadMonster(QString id)
{
    QPixmap ret = intLoadPixmap(id);
    if (ret.isNull()) return QPixmap(":/core/images/unknown_monster");
    return ret;
}

QPixmap ResourcePool::loadCharacterFigure(QString id)
{
    QPixmap ret = intLoadPixmap(id + "_figure");
    //if (ret.isNull()) return QPixmap(":/core/images/unknown_monster");
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

QString ResourcePool::getIdFromEntry(QString e)
{
    int ls = e.lastIndexOf('/');
    int ps = e.lastIndexOf('.');
    if (ls > ps) {
        qCritical() << "Cannot determine extension for entry" << e << ". Ignoring";
        return QString::null;
    }
    QString fname = e.mid(ls+1, ps-ls-1);
    QString ext = e.mid(ps+1).toLower();

    qDebug() << "id:" << fname << "type:" << ext;
    return fname;
}

ResourcePool::ResourcePool()
    : m_mainFontLoaded(false)
{

}

QByteArray ResourcePool::intLoadResource(QString id)
{
    if (id.isEmpty() || !m_resPaths.contains(id)) {
        return QByteArray();
    }

    ResourceDef rd = m_resPaths[id];
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

QPixmap ResourcePool::intLoadPixmap(QString id)
{
    if (m_pixmapCache.contains(id)) return m_pixmapCache[id];

    QByteArray ba = intLoadResource(id);
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
    m_pixmapCache[id] = ret;
    return ret;
}
