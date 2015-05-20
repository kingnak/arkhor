var masOption = game.quickOption({
	costs: [
		[{ type: Constants.Costs.MonsterToughness, amount: 10 }],
		[{ type: Constants.Costs.GateTrophy, amount: 2}],
		[
			{ type: Constants.Costs.MonsterToughness, amount: 5 },
			{ type: Constants.Costs.GateTrophy, amount: 1}
		],
	],
	name: "Recruit",
	description: "Instead of having an encounter here, "+
		"you may spend 10 toughness worth of monster trophies, "+
		"2 gate trophies, or 5 toughness worth of monster trophies "+
		"and 1 gate trophy to take 1 Ally of your choice from "+
		"the Ally deck.",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
		// TODO Let decide
        var al = game.context.drawObject(Constants.ObjectType.Ally);
		game.context.character.addToInventory(al);
	}
});
game.addFieldOption(Constants.Fields.SS_MasBoardingHouse, masOption.id);

// Encounters
var mas_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.SS_MasBoardingHouse,
	description: "After supper while sitting on the "+
		"porch you strike up a conversation with another "+
		"guest: Ryan Dean, a traveling salesman. You may "+
		"make a <b>Will (+0) check</b>. If you pass, Ryan "+
		"has a deal for you. You may draw either 1 Common "+
		"Item or 1 Unique Item and purchase it at list price. "+
		"If you fail, stay here next turn listening to "+
		"bawdy stories and tall tales.",
	options: [
		game.quickOption({
			name: "Talk to Ryan (not fully implemented)",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Talk to Ryan", Constants.Skills.Will, 0, 1);
				if (res) {
					// TODO: Let decide
                    var ob = game.context.drawObject(Constants.ObjectType.UniqueItem);
					// TODO: Pay
					game.context.character.addToInventory(ob);
				} else {
					game.context.character.delay();
				}
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(mas_enc1);

var mas_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.SS_MasBoardingHouse,
	description: "Chanting neighbors keep you up all "+
		"night. Pass a <b>Luck (-1) check</b> or lose your "+
		"choice of 1 Stamina or 1 Sanity.",
	options: [
		game.quickOption({
			name: "Try to sleep",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Try to sleep", Constants.Skills.Luck, -1, 1);
				if (!res) {
					// TODO: Let decide
					if (game.context.character.sanity > game.context.character.stamina) {
						game.context.character.damageSanity(1);
					} else {
						game.context.character.damageStamina(1);
					}
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(mas_enc2);
