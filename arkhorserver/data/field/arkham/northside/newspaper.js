// Encounters
var news_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.NS_Newspaper,
	description: "Earn $5 for a story.",
	options: [
		game.quickOption({
			name: "Sell Story",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.context.character.addMoney(5);
			}
		}).id
	]
});
game.registerArkhamEncounter(news_enc1);

var news_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.NS_Newspaper,
	description: "Editor Doyle Jefferies offers you a "+
		"Retainer in return for your fascinating stories. "+
		"Take a Retainer card.",
	options: [
		game.quickOption({
			name: "Sell Story",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
                var ret = game.drawSpecificObject("OBJ_RETAINER");
				game.context.character.addToInventory(ret);
			}
		}).id
	]
});
game.registerArkhamEncounter(news_enc2);
