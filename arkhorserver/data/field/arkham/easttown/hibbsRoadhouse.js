// Encounters
var hibbs_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.ET_HibbsRoadhouse,
	description: "\"So, what's your story, friend?\" "+
		"A smiling man inquires about your adventures over "+
		"a glass of gin. You tell him your story. If you "+
		"spend 3 Clue Tokens, he introduces himself as "+
		"Ryan Dean and asks to join you. Take his Ally card. "+
		"If it's not available, he gives you some useful "+
		"items instead. Draw 2 Common Items.",
	options: [
		game.quickOption({
			name: "Join",
			costs: { type: Constants.Costs.Clue, amount: 3},
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
                var ryan = game.drawSpecificObject("AL_Ryan_Dean");
				if (ryan.isNull()) {
					var o1 = game.drawObject(Constants.ObjectType.CommonItem);
					var o2 = game.drawObject(Constants.ObjectType.CommonItem);
					game.context.character.addToInventory(o1);
					game.context.character.addToInventory(o2);
				} else {
					game.context.character.addToInventory(ryan);
				}
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(hibbs_enc1);

var hibbs_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.ET_HibbsRoadhouse,
	description: "A horrible monster appears!",
	options: [
		game.quickOption({
			name: "Fight",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.context.spontaneousMonsterFight();
			}
		}).id
	]
});
game.registerArkhamEncounter(hibbs_enc2);
