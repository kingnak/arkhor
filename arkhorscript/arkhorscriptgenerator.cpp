#include "arkhorscriptgenerator.h"
#include <QTextStream>
#include <QDebug>

#include "commonitemgenerator.h"
#include "uniqueitemgenerator.h"
#include "allygenerator.h"
#include "skillgenerator.h"
#include "spellgenerator.h"
#include "specialobjectgenerator.h"
#include "monstergenerator.h"
#include "optiongenerator.h"
#include "actiongenerator.h"
#include "quickoptiongenerator.h"
#include "fieldoptiongenerator.h"
#include "arkhamencountergenerator.h"
#include "otherworldencountergenerator.h"
#include "mythosgenerator.h"
#include "investigatorgenerator.h"
#include "ancientonegenerator.h"
#include "blessingcursegenerator.h"

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
    m_generators["UniqueItem"] = new UniqueItemGenerator(m_out);
    m_generators["Ally"] = new AllyGenerator(m_out);
    m_generators["Spell"] = new SpellGenerator(m_out);
    m_generators["Skill"] = new SkillGenerator(m_out);
    m_generators["SpecialObject"] = new SpecialObjectGenerator(m_out);
    m_generators["Monster"] = new MonsterGenerator(m_out);
    m_generators["Option"] = new OptionGenerator(m_out);
    m_generators["Action"] = new ActionGenerator(m_out);
    m_generators["QuickOption"] = new QuickOptionGenerator(m_out);
    m_generators["FieldOption"] = new FieldOptionGenerator(m_out);
    m_generators["ArkhamEncounter"] = new ArkhamEncounterGenerator(m_out);
    m_generators["OtherWorldEncounter"] = new OtherWorldEncounterGenerator(m_out);
    m_generators["Rumor"] = new RumorGenerator(m_out);
    m_generators["Headline"] = new HeadlineGenerator(m_out);
    m_generators["Investigator"] = new InvestigatorGenerator(m_out);
    m_generators["AncientOne"] = new AncientOneGenerator(m_out);
    m_generators["BlessingCurse"] = new BlessingCurseGenerator(m_out);
    //m_generators["Curse"] = new BlessingCurseGenerator(m_out);
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
