// Encounters
var grave_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.RT_Graveyard,
	description: "<b>\"Testifying\" Cooter Falwell</b> "+
		"latches onto you and rambles on about his "+
		"spiritual beliefs. Make a <b>Lore (-1) check</b>. "+
		"If you pass, then somewhere in Cooter's words you "+
		"find a clue to the Mythos threat. Gain 1 Clue token, "+
		"but lose 1 Sanity. If you fail, move to the "+
		"street while you listen to Cooter ramble on about "+
		"pure nonsense.",
	options: [
		game.quickOption({
			name: "Listen",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Listen", Constants.Skills.Lore, -1, 1);
				if (res) {
					game.context.character.addClue(1);
					game.context.character.damageSanity(1);
				} else {
					game.context.character.placeOnField(Constants.Fields.RT_RiverTown);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(grave_enc1);

var grave_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.RT_Graveyard,
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
game.registerArkhamEncounter(grave_enc2);
