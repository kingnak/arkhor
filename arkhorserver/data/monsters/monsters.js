/*
var m = game.createMonster({
	id: "MO_BYAKHEE",
	name: "Byakhee",
    mythText:
        "It shrieked and flapped its wings. "+
		"I thought of the empty shell of a beetle, "+
		"and the soft loam of rotting earth. It cocked "+
		"its carapaced head at me, black teeth clicking "+
        "against each other.",
    //dimension: Constants.Dimension.Square,
    movementType: Constants.Movement.Fast,
	awareness: -2,
	horrorAdjustment: -1,
	horrorDamage: 1,
	combatAdjustment: 0,
	combatDamage: 2,
	toughness: 1,
	//attributes: 
});

game.registerMonster(10, m);
*/
m = game.createMonster({
    id: "MO_TEST",
    name: "Test Monster",
    description: "MEIN TEST!",
    //dimension: Constants.Dimension.Circle,
    movementType: Constants.Movement.Flying,
    awareness: -2,
    horrorAdjustment: -1,
    horrorDamage: 2,
    combatAdjustment: -1,
    combatDamage: 2,
    toughness: 1,
    attributes: Constants.Monster.Endless,
                           /*
	onMove: function() {
		game.context.character.addClue(1);
    }*/
});

game.registerMonster(10, m);

