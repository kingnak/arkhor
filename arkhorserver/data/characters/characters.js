game.registerOption(game.createOption({
  id: "OP_Dummy",
  //name: "Dummy Option",
  //phases: Constants.GamePhases.Upkeep,
  actionId: game.registerAction(game.createAction({
    id: "AC_Dummy",
    name: "Dummy Action",
    phases: Constants.GamePhases.Upkeep,
    activate: function() {}
  })).id,
  costs:

    [
        [
            {type: Constants.Costs.Money, amount: 5},
            //{type: Constants.Costs.Clue, amount: 1}
       // ],
       // [
            {type: Constants.Costs.GateTrophy, amount: 3}
        ]
    ]

/*
  [
      {type: Constants.Costs.Money, amount: 5},
      {type: Constants.Costs.Clue, amount: 1}
  ]
*/
    //{type: Constants.Costs.Money, amount: 5}
}));

game.registerInvestigator(game.createInvestigator({
  id: "CH_Jenny_Barnes",
  name: "Jenny Barnes",
  occupation: "The Dilittante",
  homeFieldId: Constants.Fields.NS_TrainStation,
  sanity: 6,
  stamina: 4,
  money: 10,
  focus: 1,
  
  uniqueAbilityId: game.registerAction(game.createAction(
    {
        id: "AC_Jenny_Barnes_unique",
        name: "Trust fund",
        phases: Constants.GamePhases.Upkeep,
        activate: function() {
            game.context.character.addMoney(1);
        }
    }

  )).id,

  randomPossessions: Array(
    {type: Constants.ObjectType.CommonItem, amount: 2},
    {type: Constants.ObjectType.UniqueItem, amount: 1},
    {type: Constants.ObjectType.Spell, amount: 1},
    {type: Constants.ObjectType.Skill, amount: 1}
  ),
  attributes: {
    SpeedSneak: Array(
      Array(0,4),
      Array(1,3),
      Array(2,2),
      Array(3,1)
    ),
    FightWill: Array(
      Array(1,5),
      Array(2,4),
      Array(3,3),
      Array(4,2)
    ),
    LoreLuck: Array(
      Array(1,5),
      Array(2,4),
      Array(3,3),
      Array(4,2)
    )
  }
}));

game.registerInvestigator(game.createInvestigator({
  id: "CH_Sister_Mary",
  name: "Sister Mary",
  occupation: "The Nun",
  homeFieldId: Constants.Fields.SS_SouthChurch,
  sanity: 4,
  stamina: 3,
  money: 0,
  focus: 1,
  fixedPossesionObjectIds: Array(
    "BC_Blessing",
    "CI_Cross",
    "UI_Holy_Water"
  ),
  randomPossessions: Array(
    {type: Constants.ObjectType.Spell, amount: 2},
    {type: Constants.ObjectType.Skill, amount: 1}
  ),
  attributes: {
    SpeedSneak: Array(
      Array(1,4),
      Array(2,3),
      Array(3,2),
      Array(4,1)
    ),
    FightWill: Array(
      Array(0,4),
      Array(1,3),
      Array(2,2),
      Array(3,1)
    ),
    LoreLuck: Array(
      Array(1,6),
      Array(2,5),
      Array(3,4),
      Array(4,3)
    )
  }
}));

