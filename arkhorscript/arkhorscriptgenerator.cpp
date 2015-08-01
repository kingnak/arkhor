#include "arkhorscriptgenerator.h"
#include <QTextStream>
#include <QDebug>

#include "commonitemgenerator.h"
#include "skillgenerator.h"

namespace AHS {

ArkhorScriptGenerator::ArkhorScriptGenerator(QIODevice *input, QIODevice *output)
{
    m_parser = new ArkhorScriptParser(input);
    m_out.setDevice(output);
    init();
}

ArkhorScriptGenerator::~ArkhorScriptGenerator()
{
    delete m_parser;
    qDeleteAll(m_generators.values());
}

QString ArkhorScriptGenerator::error() const
{
    QString errs;
    foreach (ClassGenerator *g, m_generators.values()) {
        errs += g->error();
    }

    return errs + m_error + m_parser->error();
}

QString ArkhorScriptGenerator::warning() const
{
    QString warns;
    foreach (ClassGenerator *g, m_generators.values()) {
        warns += g->warning();
    }
    return warns;
}

bool ArkhorScriptGenerator::generate()
{
    if (!m_parser->parse()) return false;

    foreach (ClassDef c, m_parser->getDefinedClasses()) {
        if (!generateClass(c)) return false;
    }

    return true;
}

void ArkhorScriptGenerator::init()
{
    m_generators["CommonItem"] = new CommonItemGenerator(m_out);
    //m_generators["UniqueItem"] = &ArkhorScriptGenerator::generateUniqueItem;
    m_generators["Skill"] = new SkillGenerator(m_out);
}

bool ArkhorScriptGenerator::generateClass(const ClassDef &cls)
{
    if (m_generators.contains(cls.elemType)) {
        return m_generators[cls.elemType]->generate(cls);
    }

    return setError("Unknown ClassType: " + cls.elemType);
}

bool ArkhorScriptGenerator::setError(QString err)
{
    m_error += err + "\n";
    return false;
}


}
