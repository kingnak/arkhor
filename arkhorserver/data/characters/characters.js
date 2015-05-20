game.registerInvestigator(game.createInvestigator({
  id: "CH_Jenny_Barnes",
  name: "Jenny Barnes",
  occupation: "The Dilittante",
  homeFieldId: Constants.Fields.NS_TrainStation,
  sanity: 6,
  stamina: 4,
  money: 10,
  focus: 1,
  clues: 3,
  
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

  randomPossessions: [
    {type: Constants.ObjectType.CommonItem, amount: 2},
    {type: Constants.ObjectType.UniqueItem, amount: 1},
    {type: Constants.ObjectType.Spell, amount: 1},
    {type: Constants.ObjectType.Skill, amount: 1}
  ],
  attributes: {
    SpeedSneak: [
      [0,4],
      [1,3],
      [2,2],
      [3,1]
    ],
    FightWill: [
      [1,5],
      [2,4],
      [3,3],
      [4,2]
    ],
    LoreLuck: [
      [1,5],
      [2,4],
      [3,3],
      [4,2]
    ]
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
  fixedPossesionObjectIds: [
    "BC_Blessing",
    "CI_Cross",
    "UI_Holy_Water"
  ],
  randomPossessions: [
    {type: Constants.ObjectType.Spell, amount: 2},
    {type: Constants.ObjectType.Skill, amount: 1}
  ],
  attributes: {
    SpeedSneak: [
      [1,4],
      [2,3],
      [3,2],
      [4,1]
    ],
    FightWill: [
      [0,4],
      [1,3],
      [2,2],
      [3,1]
    ],
    LoreLuck: [
      [1,6],
      [2,5],
      [3,4],
      [4,3]
    ]
  }
}));

