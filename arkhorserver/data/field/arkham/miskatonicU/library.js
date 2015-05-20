// Encounters
var lib_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.MU_Library,
	description: "A book in a shadowy corner of the library "+
		"begins to whisper terrible things to you. "+
		"Lose 1 Sanity.",
	options: [
		game.quickOption({
			name: "Listen",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.context.character.damageSanity(1);
			}
		}).id
	]
});
game.registerArkhamEncounter(lib_enc1);

var lib_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.MU_Library,
	description: "Make a <b>Will (+0) check</b> and consult the chart below:\n\n"+
		"Successes:\n"+
		"0) Abigail tosses you out. Move to the street.\n"+
		"1) Abigail lets you into a private section of the "+
			"library where you find an ancient tome. Draw "+
			"2 Spells and keep whichever one of them you want.\n"+
		"2+) Abigail loans you one of the strange items in "+
			"the library's display case. Draw 1 Unique Item.",
	options: [
		game.quickOption({
			name: "Check",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.dieRollSkillCount("Check", Constants.Skills.Will, 0);
				if (res == 0) {
					game.context.character.placeOnField(Constants.Fields.MU_MiskatonicU);
				} else if (res == 1) {
					// TODO: Let decide...
                    var sp = game.drawObject(Constants.ObjectType.Spell);
					game.context.character.addToInventory(sp);
				} else if (res >= 2) {
                    var ob = game.drawObject(Constants.ObjectType.UniqueItem);
					game.context.character.addToInventory(ob);
				} else {
					debugger;
					// Should not come here...
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(lib_enc2);
