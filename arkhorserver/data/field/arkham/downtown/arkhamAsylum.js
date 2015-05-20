var asylumOption1 = game.quickOption({
	name: "Psychatric Care (One)",
	description: "Instead of having an encounter here, "+
		"you may recover Sanity by receiving psychiatric "+
		"care. You regain 1 Sanity for free",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
		game.context.character.addSanity(1);
	}
});

var asylumOption2 = game.quickOption({
	costs: { type: Constants.Costs.Money, amount: 2},
	name: "Psychatric Care (All)",
	description: "Instead of having an encounter here, "+
		"you may recover Sanity by receiving psychiatric "+
		"care. Pay $2 to restore your Sanity to its maximum value.",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
		game.context.character.restoreSanity();
	}
});

game.addFieldOption(Constants.Fields.DT_ArkhamAsylum, asylumOption1.id);
game.addFieldOption(Constants.Fields.DT_ArkhamAsylum, asylumOption2.id);

// Encounters
var as_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.DT_ArkhamAsylum,
    description:"In the Doctor's study, you find a book of "+
		"helpful notes gathered from inmate interviews. "+
		"Make a <b>Lore (+0) check</b> and consult the "+
		"following chart:\n\n"+
		"Successes:\n"+
		"0) Their stories fill you with horror even "+
			"as you learn a few bits of knowledge. Lose "+
			"1 Sanity and gain 1 Clue token.\n"+
		"1-2) You find several pieces of useful "+
			"information. Gain 2 Clue tokens.\n"+
		"3+) One of the interviews contains vital "+
			"information. Gain 3 Clue tokens.",
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
game.registerArkhamEncounter(as_enc1);

var as_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.DT_ArkhamAsylum,
	description: "Nurse Heather accidentally injects "+
		"you with a sleeping draught. You may make a "+
		"<b>Fight (-2) check</b> to resist. If you fail "+
		"or choose not to resist, lose your next turn and "+
		"gain 2 Sanity from the prolonged rest. If you pass, "+
		"nothing happens.",
	options: [
		game.quickOption({
			name: "Resist",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Resist injection", Constants.Skills.Fight, -2, 1);
				if (!res) {
					game.context.character.addSanity(2);
					game.context.character.delay();
					// TODO: Delay is not enough, must completly lose the round...
				}
			}
		}).id,
		game.quickOption({
			name: "Don't resist",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.context.character.addSanity(2);
				game.context.character.delay();
				// TODO: Delay is not enough, must completly lose the round...
			}
		}).id
	]
});
game.registerArkhamEncounter(as_enc2);
