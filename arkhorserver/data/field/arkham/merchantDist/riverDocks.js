var docksOption = game.quickOption({
	costs: [
		[{ type: Constants.Costs.MonsterToughness, amount: 5 }],
		[{ type: Constants.Costs.GateTrophy, amount: 1}]
	],
	name: "Shady Character",
	description: "Instead of having an encounter here, "+
		"you may spend 5 toughness worth of monster trophies "+
		"or 1 gate trophy to gain $5.",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
		game.context.character.addMoney(5);
	}
});
game.addFieldOption(Constants.Fields.MD_RiverDocks, docksOption.id);

// Encounters
var docks_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.MD_RiverDocks,
	description: "A horrific stench draws your attention "+
		"to the body of some bizarre marine creature, rotting "+
		"on the edge of the docks. As you move towards it, "+
		"an uneasy feeling grows in the pit of your stomach, "+
		"as though you are meddling with something best left "+
		"alone. Make a <b>Will (-1) check</b>. If you pass, "+
		"lose 1 Sanity. If you fail, lose 2 Sanity. In "+
		"either event, if you are not reduced to 0 Sanity, "+
		"you find something clutched in its webbed hands. "+
		"Draw 1 Unique Item.",
	options: [
		game.quickOption({
			name: "Withstand stench",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Withstand stench", Constants.Skills.Will, -1, 1);
				if (res) {
					game.context.character.damageSanity(1);
				} else {
					game.context.character.damageSanity(2);
				}
				var ok = game.context.character.commitDamage();
				if (ok) {
					var ob = game.drawObject(Constants.ObjectType.UniqueItem);
					game.context.character.addToInventory(ob);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(docks_enc1);

var docks_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.MD_RiverDocks,
	description: "As you look out across the waves, you "+
		"feel strangely compelled to throw yourself into "+
		"the ocean's watery embrace. Pass a <b>Will (+1) "+
		"check</b> or you are <em>Lost in time and space</em>.",
	options: [
		game.quickOption({
			name: "Withstand urge",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Withstand urge", Constants.Skills.Will, +1, 1);
				if (!res) {
					game.context.character.lostInSpaceAndTime();
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(docks_enc2);
