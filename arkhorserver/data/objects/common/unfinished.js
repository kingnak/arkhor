// Axe
var axe = game.createObject({
	tid: "CI_Axe",
	name: "Axe",
	type: Constants.ObjectType.CommonItem,
	hands: 1,
	price: 3,
	modifications: function() {
		// TODO: check number of hands
		var dmg = 2;
		return { property: Constants.Mods.Damage_Physical, value: dmg}
	}
});
game.registerMultiObject(2, axe);

// Bullwhip
var bullwhip = game.createObject({
	tid: "CI_Bullwhip",
	name: "Bullwhip",
	description: "<b>Any:</b> Exhaust to re-roll 1 die after "+
		"making a Combat check."+
		"\nNot implemented",
	type: Constants.ObjectType.CommonItem,
	hands: 1,
	price: 2,
	modifications: { property: Constants.Mods.Damage_Physical, value: 1}
	/*
	options: [
		game.quickOption({
			name: "Exhaust Bullwhip",
			phases: Constants.Phases.DieRoll,
			
		}).id
	]
	*/
});
game.registerMultiObject(2, bullwhip);

// Lucky Cigarette Case
var cigarette = game.createObject({
	tid: "CI_Lucky_Cigarette_Case",
	name: "Lucky Cigarette Case",
	description: "<b>Any:</b> Discard Lucky Cigarette Case to re-roll "+
		"any one Skill Check."+
		"\nNot implemented",
	type: Constants.ObjectType.CommonItem,
	price: 1,
	/*
	options: [
		game.quickOption({
			name: "Exhaust Bullwhip",
			phases: Constants.Phases.DieRoll,
			
		}).id
	]
	*/
});
game.registerMultiObject(2, cigarette);

// Shotgun
var shotgun = game.createObject({
	tid: "CI_Shotgun",
	name: "Shotgun",
	description: "<b>Any:</b> When using Shotgun in Combat, all 6's rolled count as 2 successes."+
        "Not implemented",
	type: Constants.ObjectType.CommonItem,
	hands: 2,
	price: 6,
	modifications: { property: Constants.Mods.Damage_Physical, value: 4}
	// TODO: Count 6 as 2 successes on combat check...
});
game.registerMultiObject(2, shotgun);
