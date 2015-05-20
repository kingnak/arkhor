// Encounters
var velmas_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.ET_VelmasDiner,
	description: "\"This must be where pies go when "+
		"they die.\" If you want, pay $1 to enjoy a fine "+
		"slice of cherry pie. If you do, gain 2 Stamina",
	options: [
		game.quickOption({
			name: "Take the item",
			costs: { type: Constants.Costs.Money, amount: 1 },
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.context.character.addStamina(2);
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(velmas_enc1);

var velmas_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.ET_VelmasDiner,
	description: "\"What'll it be, hon?\" Velma takes "+
		"your order. Pay up to $6 to gain that many points "+
		"split between Sanity and Stamina however you like.",
	options: [
		game.quickOption({
			name: "Place Order (not implemented)",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				// TODO
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(velmas_enc2);
