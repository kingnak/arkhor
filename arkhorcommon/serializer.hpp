#ifndef SERIALIZER_HPP
#define SERIALIZER_HPP

#include <QVariant>
#include <QVariantMap>
#include <QList>

// Qt Types + User Metatypes
template<typename T>
void operator << (QVariant &data, const T &target) {
    data = QVariant::fromValue<T>(target);
}

template<typename T>
void operator >> (const QVariant &data, T &target) {
    target = data.value<T>();
}

// Pair
template<typename F, typename S>
void operator << (QVariant &data, const QPair<F,S> &target) {
    QVariantMap m;
    m["first"] << target.first;
    m["second"] << target.second;
    data << m;
}

template<typename F, typename S>
void operator >> (const QVariant &data, QPair<F,S> &target) {
    QVariantMap m;
    data >> m;
    m["first"] >> target.first;
    m["second"] >> target.second;
}

// List
template<typename T>
void operator << (QVariant &data, const QList<T> &target ) {
    QVariantList list;
    list.reserve(target.size());
    for (int i = 0; i < target.size(); ++i) {
        QVariant item;
        item << target[i];
        list.append( item );
    }
    data = list;
}

template<typename T>
void operator >> (const QVariant &data, QList<T> &target ) {
    QVariantList list = data.toList();
    target.reserve(list.size());
    for (int i = 0; i < list.size(); ++i) {
        T item;
        list[i] >> item;
        target.append(item);
    }
}

template<typename T>
void operator << (QVariant &data, const QList<T*> &target ) {
    QVariantList list;
    list.reserve(target.size());
    for (int i = 0; i < target.size(); ++i) {
        QVariant item;
        item << *(target[i]->data());
        list.append( item );
    }
    data = list;
}

#define DECLARE_ENUM_SERIALIZER(E) \
    void operator <<(QVariant &, const E &); \
    void operator >>(const QVariant &, E &)

#define DECLARE_ENUM_SERIALIZER_EXPORT(EXPORTER,E) \
    EXPORTER void operator <<(QVariant &, const E &); \
    EXPORTER void operator >>(const QVariant &, E &)

#define DEFINE_ENUM_SERIALIZER(E) \
    void operator <<(QVariant &data, const E &e) { \
        data << static_cast<qint32> (e); \
    } \
    void operator >>(const QVariant &data, E &e) { \
        qint32 d; \
        data >> d; \
        e = static_cast<E> (d); \
    }

/* Do with ENUM
#define DECLARE_FLAGS_SERIALIZER(F) \
    void operator <<(QVariant &, const QFlags<F> &); \
    void operator >>(const QVariant &, QFlags<F> &)

#define DECLARE_FLAGS_SERIALIZER_EXPORT(EXPORTER,F) \
    EXPORTER void operator <<(QVariant &, const QFlags<F> &); \
    EXPORTER void operator >>(const QVariant &, QFlags<F> &)

#define DEFINE_FLAGS_SERIALIZER(F) \
    void operator <<(QVariant &data, const QFlags<F>)
*/

#define DECLARE_SERIALIZABLE(cls) \
    friend void operator <<(QVariant &, const cls &);\
    friend void operator >>(const QVariant &, cls &)

#define DECLARE_SERIALIZABLE_EXPORT(EXPORTER, cls) \
    EXPORTER friend void operator <<(QVariant &, const cls &);\
    EXPORTER friend void operator >>(const QVariant &, cls &)

#endif // SERIALIZER_HPP
