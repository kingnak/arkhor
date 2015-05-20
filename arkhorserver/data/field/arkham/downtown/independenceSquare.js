// Encounters
var square_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.DT_IndependenceSquare,
	description: "A pair of friendly picnickers share "+
		"their lunch with you. Gain 1 Stamina.",
	options: [
		game.quickOption({
			name: "Go picnick",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.context.character.addStamina(1);
			}
		}).id
	]
});
game.registerArkhamEncounter(square_enc1);

var square_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.DT_IndependenceSquare,
	description: "A shadow falls across you from no "+
		"apparent source and you shiver with more than "+
		"just cold. Pass a <b>Will (-1) check</b> or lose "+
		"1 Stamina and 1 Sanity.",
	options: [
		game.quickOption({
			name: "Withstand shadow",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Withstand shadow", Constants.Skills.Will, -1, 1);
				if (!res) {
					game.context.character.damageStamina(1);
					game.context.character.damageSanity(1);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(square_enc2);