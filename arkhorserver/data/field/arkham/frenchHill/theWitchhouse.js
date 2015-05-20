// Encounters
var witch_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.FH_TheWitchHouse,
	description: "\"Excuse me, stranger, but have you ever "+
		"seen this symbol before?\" A man standing near the "+
		"house holds up an occult symbol. Make a <b>Lore (-1) "+
		"check</b>. If you pass, the man introduces himself "+
		"as <b>Thomas F. Malone</b>, a police detective visiting "+
		"Arkham on a case. He's impressed with you and "+
		"offers to join you. Take his Ally card. If it's "+
		"not available, he tells you some valuable "+
		"information instead. Gain 2 Clue tokens. If you "+
		"fail, nothing happens.",
	options: [
		game.quickOption({
			name: "Check symbol",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Check symbol", Constants.Skills.Lore, -1, 1);
				if (res) {
                    var thomas = game.drawSpecificObject("AL_Thomas_F_Malone");
					if (thomas) {
						game.context.character.addToInventory(thomas);
					} else {
						game.context.character.addClue(2);
					}
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(witch_enc1);

var witch_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.FH_TheWitchHouse,
	description: "A gate and a monster appear! ",
	options: [
		game.quickOption({
			name: "Gate and Monster appear",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				// Gate will also create monster
				game.createGate(Constants.Fields.FH_TheWitchHouse);
			}
		}).id
	]
});
game.registerArkhamEncounter(witch_enc2);
