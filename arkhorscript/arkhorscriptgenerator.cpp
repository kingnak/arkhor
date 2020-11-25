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
#include "specialabilitygenerator.h"

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
    for (auto g : m_generators) {
        errs += g->error();
    }

    return errs + m_error + m_parser->error();
}

QString ArkhorScriptGenerator::warning() const
{
    QString warns;
    for (auto g : m_generators) {
        warns += g->warning();
    }
    return warns;
}

bool ArkhorScriptGenerator::generate()
{
    if (!m_parser->parse()) return false;

    for (auto c : m_parser->getDefinedClasses()) {
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
    m_generators["Environment"] = new EnvironmentGenerator(m_out);
    m_generators["Investigator"] = new InvestigatorGenerator(m_out);
    m_generators["AncientOne"] = new AncientOneGenerator(m_out);
    m_generators["BlessingCurse"] = new BlessingCurseGenerator(m_out);
    m_generators["SpecialAbility"] = new SpecialAbilityGenerator(m_out);
    m_generators["AEOption"] = new ArkhamEncounterOptionGenerator(m_out);
    m_generators["OWEOption"] = new OtherWorldEncounterOptionGenerator(m_out);
}

bool ArkhorScriptGenerator::generateClass(ClassDef &cls)
{
    // Generate nested objects and replace by ID
    generateNestedClasses(cls);

    if (m_generators.contains(cls.elemType)) {
        if (!m_generators[cls.elemType]->fixClass(cls)) return false;
        return m_generators[cls.elemType]->generate(cls);
    }

    return setError("Unknown ClassType: " + cls.elemType);
}

bool ArkhorScriptGenerator::setError(const QString &err)
{
    m_error += err + "\n";
    return false;
}

bool ArkhorScriptGenerator::generateNestedClasses(ClassDef &cls)
{
    for (auto &a : cls.attrs) {
        if (a.type == ArkhorScriptParser::NestedObject) {
            if (!doGenerateNestedClass(a)) {
                return false;
            }
        } else if (a.type == ArkhorScriptParser::ArrayValues) {
            for (auto &e : a.array) {
                if (e.first == ArkhorScriptParser::NestedObject) {
                    AttrDef tmp = AttrDef::forArrayElement(e);
                    if (!doGenerateNestedClass(tmp)) {
                        return false;
                    }
                    e.second = tmp.content;
                    e.first = tmp.type;
                }
            }
        }
    }
    return true;
}

bool ArkhorScriptGenerator::doGenerateNestedClass(AttrDef &attr)
{
    if (!generateClass(*attr.content.second)) {
        return false;
    }
    attr.content.first = ClassGenerator::constantScopeForClass(attr.content.second->elemType) + "." + attr.content.second->elemName;
    attr.type = ArkhorScriptParser::IDRef;
    return true;
}

}
