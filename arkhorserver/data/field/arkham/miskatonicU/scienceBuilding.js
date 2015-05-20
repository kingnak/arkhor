var scibiOption = game.quickOption({
	costs: [
		[{ type: Constants.Costs.MonsterToughness, amount: 5 }],
		[{ type: Constants.Costs.GateTrophy, amount: 1}]
	],
	name: "Dissection",
	description: "Instead of having an encounter here, "+
		"you may spend 5 toughness worth of monster trophies "+
		"or 1 gate trophy to gain 2 Clue tokens.",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
		game.context.character.addClue(2);
	}
});
game.addFieldOption(Constants.Fields.MU_ScienceBuilding, scibiOption.id);

// Encounters
var scibi_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.MU_ScienceBuilding,
	description: "A chemical brew bubbles on a nearby "+
		"Bunsen burner. It smells delicious. If you drink "+
		"it, make a <b>Luck (+0) check</b>. If you pass, "+
		"the strange liquid fortifies you. Roll a die and "+
		"gain that many points, split between your Stamina "+
		"and Sanity however you like. If you fail, the "+
		"liquid turns out to be coffee. Gain 1 Stamina.",
	options: [
		game.quickOption({
			name: "Drink",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				debugger;
				var res = game.context.skillTest("Drink", Constants.Skills.Luck, 0, 1);
				if (res) {
					var ct = game.context.genericDieRollSum("Points to gain", 1);
					// TODO: Let user decide how to split
					var v1 = ct/2;
					var v2 = ct - v1;
					if (game.context.sanity < game.context.stamina) {
						game.context.character.addStamina(v1);
						game.context.character.addSanity(v2);
					} else {
						game.context.character.addStamina(v2);
						game.context.character.addSanity(v1);
					}
				} else {
					game.context.character.addStamina(1);
				}
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(scibi_enc1);

var scibi_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.MU_ScienceBuilding,
	description: "A professor of the occult asks you to "+
		"hold a hideous statue that he believes to have "+
		"strange powers while he reads a scroll. Energy "+
		"shoots through your body. Make a <b>Luck (-1) check</b>. "+
		"If you pass, your spirit rises from your body and "+
		"you feel that you have the power to switch bodies "+
		"with another investigator. You may choose another "+
		"investigator from the pile of unused investigators "+
		"and bring it into play as a new character, "+
		"discarding your current investigator (along with "+
		"all of his items, skills, trophies, etc.). If you "+
		"fail, nothing happens.",
	options: [
		game.quickOption({
			name: "Not implemented",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				// TODO
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(scibi_enc2);
