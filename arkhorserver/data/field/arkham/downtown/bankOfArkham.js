var bankOption = game.quickOption({
	name: "Bank Loan",
	description: "Instead of having an encounter here, "+
		"you may take out a Bank Loan if you don't have "+
		"one yet",
	phases: Constants.GamePhases.ArkhamEncountery,
	activate: function() {
		var loan = game.drawSpecificObject("OBJ_BANK_LOAN");
		game.context.character.addToInventory(loan);
	}
});
game.addFieldOption(Constants.Fields.DT_BankOfArkham, bankOption.id);

// Encounters
var bank_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.DT_BankOfArkham,
	description: "\"This is a stick-up, see? Nobody move!\" "+
		"Three men armed with tommy guns rob the bank while "+
		"you're standing in line. Make a <b>Combat (-1) check</b>. "+
		"If you pass, you drive them off. Nothing happens. "+
		"If you fail, lose all of your money.",
	options: [
		game.quickOption({
			name: "Bank Robbery",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Drive off", Constants.Skills.Combat, -1, 1);
				if (!res) {
					game.context.character.loseMoney(10000);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(bank_enc1);

// Encounters
var bank_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.DT_BankOfArkham,
	description: "A little old lady stands in front "+
		"of you in line counting out a bag of pennies "+
		"to deposit. Lose 1 Sanity.",
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
game.registerArkhamEncounter(bank_enc2);
