var shopOption = game.quickOption({
    name: "Shop",
	description: "Instead of having an encounter here, "+
		"you may draw 3 Unique Items and purchase one of "+
		"them for its list price. Discard the other two "+
		"items."+
		"\nNot implemented",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
        debugger;
        var res = game.context.drawMultipleObjects(Constants.ObjectType.UniqueItem, "Buy Items", 3, 0, 1);
        if (res.length > 0) {
            var item = res[0];
            var costs = { type: Constants.Costs.Money, amount: item.price};
            if (game.context.character.pay(costs)) {
                game.context.character.addToInventory(item);
            } else {
                item.returnToDeck();
            }
        }
	}
});
game.addFieldOption(Constants.Fields.NS_CuriositieShoppe, shopOption.id);

// Encounters
var shop_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.NS_CuriositieShoppe,
	description: "A pulsing void gapes behind a bookshelf, "+
		"sending out waves of heat. Pass a <b>Fight (-1) check</b> "+
		"or it sucks you in, hurling you into the Abyss. "+
		"Have one encounter there, then immediately return.",
	options: [
		game.quickOption({
			name: "Resist (not implemented)",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Resist", Constants.Skills.Fight, -1, 1);
				if (!res) {
					// TODO
				}
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(shop_enc1);

var shop_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.NS_CuriositieShoppe,
	description: "A sale takes place. All players may "+
		"participate. Turn over the top 3 Common Item cards "+
		"and the top Unique Item card. Any player may buy "+
		"one or more of these cards for their list price. "+
		"If there is a disagreement over who gets to buy a "+
		"certain card, you decide. Any items not sold are "+
		"discarded.",
	options: [
		game.quickOption({
			name: "Sale (not implemented)",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				// TODO
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(shop_enc2);
