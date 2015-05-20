// Encounters
var woods_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.UT_Woods,
	description: "A gate and a monster appear!",
	options: [
		game.quickOption({
			name: "A gate and monster appear!",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.createGate(Constants.Fields.UT_Woods);
			}
		}).id
	]
});
game.registerArkhamEncounter(woods_enc1);

var woods_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.UT_Woods,
	description: "You are bushwhacked by the Sheldon Gang. "+
		"Pass a <b>Luck (-1) check</b> to avoid their trap. "+
		"If you fail, lose 2 items of your choice and 2 Stamina.",
	options: [
		game.quickOption({
			name: "Avoid trap",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Avoid trap", Constants.Skills.Luck, -1, 1);
				if (!res) {
					// TODO: Choose items to lose
					game.context.character.damageStamina(2);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(woods_enc2);
