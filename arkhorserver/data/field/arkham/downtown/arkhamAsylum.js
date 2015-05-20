var asylumOption1 = game.quickOption({
    name: "Psychatric Care (One)",
    description: "Instead of having an encounter here, you may recover Sanity by receiving psychiatric care. You regain 1 Sanity for free",
    phases: Constants.GamePhases.ArkhamEncountery,
    activate: function() {
        game.context.character.addSanity(1);
    }
});

var asylumOption2 = game.quickOption({
    costs: { type: Constants.Costs.Money, amount: 2},
    name: "Psychatric Care (All)",
    description: "Instead of having an encounter here, you may recover Sanity by receiving psychiatric care. Pay $2 to restore your Sanity to its maximum value.",
    phases: Constants.GamePhases.ArkhamEncountery,
    activate: function() {
        game.context.character.restoreSanity();
    }
});

game.addFieldOption(Constants.Fields.DT_ArkhamAsylum, asylumOption1.id);
game.addFieldOption(Constants.Fields.DT_ArkhamAsylum, asylumOption2.id);

// Encounters
var enc1 = game.createArkhamEncounter({
    field: Constants.Fields.DT_ArkhamAsylum,
    description:"In the Doctor's study, you find a book of helpful notes gathered from inmate interviews. Make a Lore (+0) check and consult the following chart:\n\n"+
                "Successes:\n"+
                "0) Their stories fill you with horror even as you learn a few bits of knowledge. Lose 1 Sanity and gain 1 Clue token.\n"+
                "1-2) You find several pieces of useful information. Gain 2 Clue tokens.\n"+
                "3+) One of the interviews contains vital information. Gain 3 Clue tokens.",
    options: [
        game.quickOption({
            name: "Arkham Asylum Encountery",
            phases: Constants.GamePhases.ArkhamEncountery,
            activate: function() {
                var res = game.context.dieRollSkillCount("", Constants.Skills.Lore, 0);
                switch (res) {
                case 0:
                    game.context.character.addClue(1);
                    game.context.character.damageSanity(1);
                    break;
                case 1:
                case 2:
                    game.context.character.addClue(2);
                    break;
                default:
                    game.context.character.addClue(3);
                }
            }
        }).id
    ]
});

game.registerArkhamEncounter(enc1);
