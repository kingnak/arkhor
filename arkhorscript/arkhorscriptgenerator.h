#ifndef ARKHORSCRIPTGENERATOR_H
#define ARKHORSCRIPTGENERATOR_H

#include "arkhorscript_global.h"
#include "arkhorscriptparser.h"
#include <QMap>
#include <QTextStream>

namespace AHS {

class ClassGenerator;

class ARKHORSCRIPTSHARED_EXPORT ArkhorScriptGenerator
{
public:
    ArkhorScriptGenerator(QIODevice *input, QIODevice *output);
    ~ArkhorScriptGenerator();

    QString error() const;
    QString warning() const;
    bool generate();

private:
    void init();

    typedef ArkhorScriptParser::ClassDef ClassDef;
    typedef ArkhorScriptParser::AttrDef AttrDef;

    bool generateClass(ClassDef &cls);
    bool setError(QString err);

private:
    QMap<QString, ClassGenerator*> m_generators;

private:
    ArkhorScriptParser *m_parser;
    QTextStream m_out;
    QString m_error;
};

}

#endif // ARKHORSCRIPTGENERATOR_H
