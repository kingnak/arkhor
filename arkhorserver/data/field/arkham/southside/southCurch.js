var churchOption = game.quickOption({
	costs: [
		{ type: Constants.Costs.MonsterToughness, amount: 5 },
		{ type: Constants.Costs.GateTrophy, amount: 1}
	],
	name: "Blessing",
    description: "Instead of having an encounter here, "+
		"you may spend 5 toughness worth of monster trophies "+
		"or 1 gate trophy to have any investigator you "+
		"choose be Blessed.",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
		// TODO Let decide
		var blessing = game.drawSpecificObject("BC_BLESSING");
		game.context.character.addToInventory(blessing);
	}
});
game.addFieldOption(Constants.Fields.SS_SouthChurch, churchOption.id);

// Encounters
var church_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.SS_SouthChurch,
	description: "Father Michael convinces you that there "+
		"are members of his congregation in greater need "+
		"than you. Donate either half your money (rounded up) "+
		"or half your items (your choice, rounded up) to "+
		"the poor.",
	options: [
		game.quickOption({
			name: "Donate (not implemented)",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				// TODO Let decide
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(church_enc1);

// Encounters
var church_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.SS_SouthChurch,
	description: "Knowing that you are engaged in God's "+
		"work, Father Michael <em>Blesses</em> you.",
	options: [
		game.quickOption({
			name: "get blessed (not implemented)",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				debugger;
                var blessing = game.drawSpecificObject("BC_BLESSING");
				game.context.character.addToInventory(blessing);
			}
		}).id
	]
});
game.registerArkhamEncounter(church_enc2);
