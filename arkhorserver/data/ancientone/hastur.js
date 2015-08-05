game.registerAncientOne(game.createAncientOne({
    id: "AO_HASTUR",
    name: "Hastur",
    combatAdjustment: 0,
    worshippersText: "Hastur's worshippers ride byakhee mounts that they "+
                     "call with enchanted whistles. <b>Cultists</b> are <em>flying "+
                     "monsters</em> and their combat rating is -2.",
    powerTitle: "The King in Yellow",
    powerText: "While Hastur stirs in his slumber, the cost to seal a gate is 8 Clue tokens instead of 5.",
    battleStartText: "Combat adjustment is set to the current terror level",
    attackText: "Each investigator must pass a Luck (+1) check or lose 2 Sanity. "+
                "This check's modifier decreases by 1 each turn (+0 the 2nd turn, -1 the 3rd turn, etc.)",
    doomTrack: 13,
    monsterModifications: [
        { id: "MO_Cultist", mod: [
            {property: Constants.Mods.Monster_CombatAdjustment, value: -3},
            {property: Constants.Mods.Monster_Movement, value: "="+Constants.Movement.Flying}
        ] }
    ],
    defenses: Constants.Monster.PhysicalResistance,
    slumberModifications: {property: Constants.Mods.Game_SealClueCost, value: 3},

    onAwake: function() {
        this.setCombatAdjustment(game.context.terrorLevel);
    },
    attackAdjustment: 1,

    attack: function() {
        var res = game.context.skillTest("Hastur attacks", Constants.Skills.Luck, this.attackAdjustment, 1);
        if (!res) {
            game.context.character.damageSanity(2);
        }
    }

}));
