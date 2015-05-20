#ifndef DYNAMICPROPERTYSCRIPT_H
#define DYNAMICPROPERTYSCRIPT_H

#include <dynamicpropertydata.h>
#include <QScriptValue>

class DynamicScriptableObject;

class DynamicPropertyScript : public AH::Common::DynamicPropertyData
{
public:
    DynamicPropertyScript();

    void setObject(QScriptValue v);

    virtual QString display() const;
    QByteArray propertyName() const { return m_propertyName; }

    static bool createDynamicProperties(QScriptValue v, DynamicScriptableObject *o);
    static bool parseDynamicProperty(QString name, QScriptValue v, DynamicPropertyScript &o);

private:
    //void appendToObject(QObject *obj);
    QByteArray m_propertyName;
    QScriptValue m_this;
    QScriptValue m_toString;
};

//Q_DECLARE_METATYPE(DynamicPropertyScript)

class DynamicScriptableObject : public QObject
{
public:
    explicit DynamicScriptableObject(QObject *parent) : QObject(parent) {}
    void addDynamicProperty(DynamicPropertyScript prop);
    QList<AH::Common::DynamicPropertyData> getDynamicPropertyDataList() const;

protected:
    QMap<QByteArray, DynamicPropertyScript> m_dynamicProperties;

    void clonePropertiesInto(DynamicScriptableObject *other);
    void resolveDependencies(QScriptValue thisObj);

    virtual void dynamicPropertyChanged() {}

    bool event(QEvent *eve);

};

#endif // DYNAMICPROPERTYSCRIPT_H