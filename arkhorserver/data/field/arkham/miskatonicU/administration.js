var admOption = game.quickOption({
	costs: { type: Constants.Costs.Money, amount: 8},
	name: "Classes",
	description: "Instead of having an encounter here, you "+
		"may pay $8 to draw 2 Skills. Keep one of them and "+
		"discard the other.",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
        var sk = game.context.drawMultipleObjects(Constants.ObjectType.Skill, "Select Skill", 2, 1, 1);
        if (sk.length > 0) {
            game.context.character.addToInventory(sk[0]);
        }
	}
});
game.addFieldOption(Constants.Fields.MU_Administration, admOption.id);

// Encounters
var adm_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.MU_Administration,
	description: "A student mistakes you for the bursar. If "+
		"you want to carry on the deception, make a "+
		"<b>Will (-2) check</b>. If you pass, gain $8 in "+
		"ill-gotten tuition money. If you fail, you're "+
		"arrested and taken to the Police Station.",
	options: [
		game.quickOption({
			name: "Carry on",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Carry on deception", Constants.Skills.Will, -2, 1);
				if (res) {
					game.context.character.addMoney(8);
				} else {
					game.context.character.arrest();
				}
			}
		}).id,
		"OP_SKIP"
	]
});
game.registerArkhamEncounter(adm_enc1);

var adm_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.MU_Administration,
	description: "Discuss the opportunity to sell a "+
		"monograph with the President of the University. "+
		"Pass a <b>Lore (-1) check</b> to make the sale "+
		"and gain $5.",
	options: [
		game.quickOption({
			name: "Discuss",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Discuss sale", Constants.Skills.Lore, -1, 1);
				if (res) {
					game.context.character.addMoney(5);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(adm_enc2);
