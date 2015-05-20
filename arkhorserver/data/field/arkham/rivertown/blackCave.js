// Encounters
var cave_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.RT_BlackCave,
	description: "A monster appears!",
	options: [
		game.quickOption({
			name: "A monster appears! ",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.context.spontaneousMonsterFight();
			}
		}).id
	]
});
game.registerArkhamEncounter(cave_enc1);

var cave_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.RT_BlackCave,
	description: "In the darkness you happen upon the "+
		"remains of a previous spelunker. Make a "+
		"<b>Luck (+0) check</b> and consult the chart below:\n"+
		"Successes:\n"+
		"0) The body begins to bloat and splits open, "+
		"releasing the horror within. Lose 1 Sanity and "+
		"a monster appears!\n"+
		"1) The body has been ripped apart as if shredded "+
		"by a powerful monster. Lose 1 Sanity.\n"+
		"2+) Searching the body you find something "+
		"intersting. Draw 1 Common Item.",
	options: [
		game.quickOption({
			name: "Check the body",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.dieRollSkillCount("Check Body", Constants.Skills.Luck, 0, 1);
				if (res == 0) {
					game.context.character.damageSanity(1);
					if (game.context.character.commitDamage()) {
						game.context.character.spontaneousMonsterFight();
					}
				} else if (res == 1) {
					game.context.character.damageSanity(1);
				} else if (res >= 2) {
					var ob = game.drawObject(Constants.ObjectType.CommonItem);
					game.context.character.addToInventory(ob);
				} else {
					debugger;
					// Should never come here
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(cave_enc2);
