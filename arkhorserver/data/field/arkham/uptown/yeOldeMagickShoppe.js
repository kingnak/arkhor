var magOption = game.quickOption({
	costs: { type: Constants.Costs.Money, amount: 5},
	name: "Magic Lessons",
	description: "Instead of having an encounter here, you "+
		"may pay $5 to draw 2 Spells. Keep one of them and "+
		"discard the other.",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
        var sp = game.context.drawMulitpleObjects(Constants.ObjectType.Spell, "Select Spell", 2, 0, 1);
        if (sp.length > 0) {
            game.context.character.addToInventory(sp[0]);
        }
	}
});
game.addFieldOption(Constants.Fields.UT_YeOldeMagickShoppe, magOption.id);

// Encounters
var mag_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.UT_YeOldeMagickShoppe,
	description: "Looking closely at a mummified head in "+
		"the shop, you are horrified to find it looking back "+
		"at you! Lose 1 Sanity.",
	options: [
		game.quickOption({
			name: "Go Crazy",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.context.character.damageSanity(1);
			}
		}).id
	]
});
game.registerArkhamEncounter(mag_enc1);

var mag_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.UT_YeOldeMagickShoppe,
	description: "Looking into a glass ball, you receive "+
		"a vision of things to come. Turn the top card of "+
		"one location deck of your choice face up. The "+
		"next investigator to have an encounter at that "+
		"location draws that encounter card.",
	options: [
		game.quickOption({
			name: "Look into glass ball (not implemented)",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				// TODO
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(mag_enc2);
