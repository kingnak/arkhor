
// Special Encounter:
game.addFieldOption(Constants.Fields.MU_Administration,
  game.registerOption(game.createOption({
    id: "OP_Field_Classes",
    costs: { type: Constants.Costs.Money, amount: 8 },
    actionId: game.registerAction(game.createAction({
        id: "AC_Field_Classes",
        name: "Classes",
        description: "Instead of having an encounter here, you may pay $8 to draw 2 Skills. Keep one of them and discard the other.",
        phases: Constants.GamePhases.ArkhamEncountery,
        activate: function() {
            game.context.drawCard(Constants.ObjectType.Skill, new DrawCardOption(2, 1));
        }
    })).id
})).id);
