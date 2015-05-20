// Encounters
var unnam_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.MD_TheUnnamable,
	description: "A monster and a gate appear!",
	options: [
		game.quickOption({
			name: "A monster and a gate appear!",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.createGate(Constants.Fields.MD_TheUnnamable);
			}
		}).id
	]
});
game.registerArkhamEncounter(unnam_enc1);

var unnam_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.MD_TheUnnamable,
	description: "In a dusty and decaying roll-top desk, "+
		"you find a mysterious manuscript. If you read it, "+
		"make a <b>Lore (-1) check</b>. If you pass, draw "+
		"1 Spell. If you fail, the manuscript is nothing "+
		"but the insane babbling of a previous renter. "+
		"Stay here next turn reading it, but gain 2 Clue "+
		"tokens.",
	options: [
		game.quickOption({
			name: "Read manuscript",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Read manuscript", Constants.Skills.Lore, -1, 1);
				if (res) {
                    var ob = game.drawObject(Constants.ObjectType.Spell);
					game.context.character.addToInventory(ob);
				} else {
					game.context.character.addClue(2);
					game.context.character.delay();
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(unnam_enc2);
