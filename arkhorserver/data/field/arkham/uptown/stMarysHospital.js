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
