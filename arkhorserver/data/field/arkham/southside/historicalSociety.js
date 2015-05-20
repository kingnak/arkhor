// Encounters
var hissoc_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.SS_HistoricalSociety,
	description: "Pass a <b>Luck (-1)[2] check</b> to "+
		"gain insight into a skill while studying the old "+
		"volumes of books. Draw 1 Skill, but stay here "+
		"next turn.",
	options: [
		game.quickOption({
			name: "Read",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Read books", Constants.Skills.Luck, -1, 2);
				if (res) {
                    var ob = game.context.drawObject(Constants.ObjectType.Skill);
					game.context.character.addToInventory(ob);
					game.context.character.delay();
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(hissoc_enc1);

var hissoc_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.SS_HistoricalSociety,
	description: "Pay $3 fee to gain access to the "+
		"private library. If you refuse, move to the "+
		"street. If you pay, make a <b>Luck (-1) check</b>. "+
		"If you pass, you learn an ancient spell from a "+
		"dusty ledger. Draw 1 Spell. If you fail, you "+
		"doze off and enter The Dreamlands. Have an "+
		"encounter there, then immediately return here.",
	options: [
		game.quickOption({
			name: "Pay",
			costs: { type: Constants.Costs.Money, amount: 3 },
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Read ledger", Constants.Skills.Luck, -1, 1);
				if (res) {
                    var ob = game.context.drawObject(Constants.ObjectType.Spell);
					game.context.character.addToInventory(ob);
				} else {
					// TODO
				}
			}
		}).id,

		game.quickOption({
			name: "Refuse",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.context.character.placeOnField(Constants.Fields.SS_SouthSide);
			}
		}).id
	]
});
game.registerArkhamEncounter(hissoc_enc2);
