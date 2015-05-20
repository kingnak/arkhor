// Encounters
var lodge_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.FH_SilverTwilightLodge,
	description: "\"Care to join the Order?\" Carl Sanford "+
		"and several of his henchmen ask. If you accept, "+
		"pay $3 and take a Silver Twilight Membership. "+
		"If you decline, pass a <b>Will (-1) check</b> or lose "+
		"3 Stamina as the henchmen assist you out the door. "+
		"Whether you pass or not, move to the street.",
	options: [
		game.quickOption({
			name: "Decline",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Go out camly", Constants.Skills.Will, -1, 1);
				if (!res) {
					game.context.character.damageStamina(3);
				}
				game.context.character.placeOnField(Constants.Fields.FH_FrenchHill);
			}
		}).id,
		game.quickOption({
			name: "Accept (not implemented)",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				// TODO
			}
		}).id
	]
});
game.registerArkhamEncounter(lodge_enc1);

var lodge_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.FH_SilverTwilightLodge,
	description: "Brushing up against a strange object "+
		"in the hall, you feel stretched and thin, like "+
		"your skin is too tight. Pass a <b>Luck (-1) check</b> "+
		"or you are <em>Cursed</em>.",
	options: [
		game.quickOption({
			name: "Resist curse",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				debugger;
				var res = game.context.skillTest("Go out camly", Constants.Skills.Luck, -1, 1);
				if (!res) {
                    var curse = game.drawSpecificObject("BC_CURSE");
					game.context.character.addToInventory(curse);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(lodge_enc2);
