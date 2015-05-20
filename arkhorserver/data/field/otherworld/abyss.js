game.registerOtherWorldEncounter(game.createOtherWorldEncounter({
    //field: Constants.Fields.OW_Abyss,
    color: Constants.OtherWorld.Blue | Constants.OtherWorld.Red|Constants.OtherWorld.Yellow|Constants.OtherWorld.Green,
	description: "A glowing light ahead draws you towards it. "+
		"Make a <b>Luck (-1) check</b>. If you pass, the glow "+
		"is a way home. Return to Arkham. If you fail, the light "+
		"is a trap set by one of the creatures that live here. "+
		"A monster appears!",
	options: [
		game.quickOption({
			name: "Check Light",
			//description: "",
			phases: Constants.GamePhases.OtherWorldEncountery,
			activate: function() {
                var res = game.context.skillTest("Check Light", Constants.Skills.Luck, -2, 1);
				if (res) {
					game.context.character.returnToArkham();
				} else {
					game.context.spontaneousMonsterFight();
				}
			}
		}).id
	]
}));
/*
game.registerOtherWorldEncounter(game.createOtherWorldEncounter({
	field: Constants.Fields.OW_Abyss,
	color: Constants.OtherWorld.Red,
	description: "The darkness seethes in pursuit, gaining... gaining... "+
		"If your Speed is currently 2 or less, you are <em>lost in time and space</em>.",
	options: [
		game.quickOption({
			name: "Check Speed",
			phases: Constants.GamePhases.OtherWorldEncountery,
			activate: function() {
				var val = game.context.character.getSkillValue(Constants.Skills.Speed);
				if (val <= 2) {
					game.context.character.lostInSpaceAndTime();
				}
			}
		}).id
	]
}));

game.registerOtherWorldEncounter(game.createOtherWorldEncounter({
	field: Constants.Fields.OW_Abyss,
	color: Constants.OtherWorld.Red,
	description: "You discover a large chamber filled with hideous creatures, "+
		"in number too vast to count. Your only hope is to pass undetected. "+
		"Pass a <b>Sneak (+1) check</b> or be <b>devoured</b>.",
	options: [
		game.quickOption({
			name: "Check Sneak",
			phases: Constants.GamePhases.OtherWorldEncountery,
			activate: function() {
				var res = game.context.skillTest("Check Sneak", Constants.Skills.Sneak, +1, 1);
				if (!res) {
					game.context.character.devour();
				}
			}
		}).id
	]
}));
*/
