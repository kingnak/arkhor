var policeOption = game.quickOption({
	costs: [
		[{ type: Constants.Costs.MonsterToughness, amount: 10 }],
		[{ type: Constants.Costs.GateTrophy, amount: 2}],
		[
			{ type: Constants.Costs.MonsterToughness, amount: 5 },
			{ type: Constants.Costs.GateTrophy, amount: 1}
		],
	],
	name: "Deputized",
	description: "Instead of having an encounter here, "+
		"you may spend 10 toughness worth of monster trophies, "+
		"2 gate trophies, or 5 toughness worth of monster trophies "+
		"and 1 gate trophy to become the Deputy of Arkham. "+
		"Take the Deputy of Arkham card.",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
		var dep = game.drawSpecificObject("OB_DEPUTY_OF_ARKHAM");
		if (dep) {
			game.context.character.addToInventory(dep);
		}
	}
});
game.addFieldOption(Constants.Fields.ET_PoliceStation, policeOption.id);

// Encounters
var police_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.ET_PoliceStation,
	description: "Deputy Dingby accidently drops a case "+
		"file as he makes his way past you. Pass a "+
		"<b>Sneak (+0) check</b> to search the Common Item "+
		"deck for a Research Materials card and take it.",
	options: [
		game.quickOption({
			name: "Take the item",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Take the item", Constants.Skills.Sneak, 0, 1);
				if (res) {
					var ob = game.drawSpecificObject("OBJ_RESEARCH_MATERIAL");
					game.context.character.addToInventory(ob);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(police_enc1);

var police_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.ET_PoliceStation,
	description: "Deputy Dingby, excitedly cleaning his "+
		"gun, fires a bullet from the chamber in your "+
		"direction. Pass a <b>Luck (-1) check</b> to avoid "+
		"getting shot. If you fail, lose 2 Stamina.",
	options: [
		game.quickOption({
			name: "Dodge the bullet",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Dodge the bullet", Constants.Skills.Luck, -1, 1);
				if (!res) {
					game.context.character.damageStamina(2);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(police_enc2);