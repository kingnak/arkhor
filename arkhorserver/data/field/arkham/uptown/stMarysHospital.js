var hospitalOption1 = game.quickOption({
	name: "Medical Care (One)",
	description: "Instead of having an encounter here, you may recover Stamina by receiving medical care. You regain 1 Stamina for free",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
		game.context.character.addStamina(1);
	}
});

var hospitalOption2 = game.quickOption({
	costs: { type: Constants.Costs.Money, amount: 2},
	name: "Medical Care (All)",
	description: "Instead of having an encounter here, you may recover Stamina by receiving medical care. Pay $2 to restore your Stamina to its maximum value.",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
		game.context.character.restoreStamina();
	}
});
game.addFieldOption(Constants.Fields.UT_StMarysHospital, hospitalOption1.id);
game.addFieldOption(Constants.Fields.UT_StMarysHospital, hospitalOption2.id);

// Encounters
var hosp_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.UT_StMarysHospital,
	description: "Make a <b>Luck (-1) check</b>. If you "+
		"pass, you realize that Dr. Mortimore is sneaking "+
		"up behind you with a hypodermic needle filled with "+
		"a phosphorescent gel. You avoid his experiment and "+
		"subdue the mad doctor. The city awards you $3 and "+
		"you gain 2 Sanity in the process. If you fail, "+
		"lose 2 Sanity, then you are dumped in the street.",
	options: [
		game.quickOption({
			name: "Detect Dr. Morimore",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Detect Dr. Morimore", Constants.Skills.Luck, -1, 1);
				if (res) {
					game.context.character.addMoney(3);
					game.context.character.addSanity(2);
				} else {
					game.context.character.damageSanity(2);
					game.context.character.placeOnField(Constants.Fields.UT_Uptown);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(hosp_enc1);

var hosp_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.UT_StMarysHospital,
	description: "Nurse Sharon slips something into your "+
		"hand when the doctor isn't looking. Pass a <b>Sneak "+
		"(-1) check</b> to keep anyone else from noticing. "+
		"If you do, you later examine the object and find "+
		"it to be an old parchment with a spell scratched "+
		"on it. Draw 1 Spell. If you fail, an orderly "+
		"takes it away from you and you gain nothing.",
	options: [
		game.quickOption({
			name: "Sneak",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Sneak", Constants.Skills.Sneak, -1, 1);
				if (res) {
                    var ob = game.context.drawObject(Constants.ObjectType.Spell);
					game.context.character.addToInventory(ob);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(hosp_enc2);
