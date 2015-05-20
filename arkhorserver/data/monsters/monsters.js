var m = game.createMonster({
	id: "MO_BYAKHEE",
	name: "Byakhee",
	description: 
		"<em>It shrieked and flapped its wings. "+
		"I thought of the empty shell of a beetle, "+
		"and the soft loam of rotting earth. It cocked "+
		"its carapaced head at me, black teeth clicking "+
		"against each other.",
	dimension: Constants.Dimension.Circle,
	movementType: Constants.Movement.Flying,
	awareness: -2,
	horrorAdjustment: -1,
	horrorDamage: 1,
	combatAdjustment: 0,
	combatDamage: 2,
	toughness: 1,
	//attributes: 
});

game.registerMonster(3, m);