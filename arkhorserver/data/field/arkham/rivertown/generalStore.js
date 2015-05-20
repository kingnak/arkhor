var storeOption = game.quickOption({
    name: "Shop",
    description: "Instead of having an encounter here, "+
      "you may draw 3 Common Items and purchase one of "+
      "them for its list price. Discard the other two items.",
    phases: Constants.GamePhases.ArkhamEncountery,
    activate: function() {
        var res = game.context.drawMultipleObjects(Constants.ObjectType.CommonItem, "Buy Items", 3, 0, 1);
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
game.addFieldOption(Constants.Fields.RT_GeneralStore, storeOption.id);

// Encounters
var store_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.RT_GeneralStore,
	description: "\"Hey, you dropped this!\" A young "+
		"street urchin hands you an item and then scampers "+
		"off. You don't recognize the item, but the boy "+
		"is already gone. Draw 1 Common Item.",
	options: [
		game.quickOption({
			name: "Take the item",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
                var ob = game.context.drawObject(Constants.ObjectType.CommonItem);
				game.context.character.addToInventory(ob);
			}
		}).id
	]
});
game.registerArkhamEncounter(store_enc1);

var store_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.RT_GeneralStore,
	description: "A jar on the counter bears a sign "+
		"proclaiming, \"Guess how many marbles are in the "+
		"jar and win a prize! $1 entry fee.\" If you want, "+
		"you may pay $1 to make a <b>Lore (-2) check</b>. "+
		"If you pass, you gain $5. If you fail, nothing happens.",
	options: [
		game.quickOption({
			name: "Guess",
			costs: { type: Constants.Costs.Money, amount: 1 },
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Guess", Constants.Skills.Lore, -2, 1);
				if (res) {
					game.context.character.addMoney(5);
				}
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(store_enc2);
