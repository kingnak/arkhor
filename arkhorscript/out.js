
// Generated CommonItem "Derringer"
var gen_Derringer = game.createObject({
	name: ".18 Derringer",
	hands: 1,
	price: 3,
	description: ".18 Derringer cannot be lost or stolen unless you choose to allow it.",
	modifications: [ { property: Constants.Mods.Damage_Physical, value: "+2" } ],
	type: Constants.ObjectType.CommonItem,
	tid: "CI_Derringer"
});
game.registerMultiObject(2, gen_Derringer);


// Generated CommonItem "Cross"
var gen_Cross = game.createObject({
	description: "+3 Combat Damage against <b>Undead</b>",
	hands: 1,
	price: 3,
	modifications: function() {
		var m = game.context.monster;
		var dmg = 0;
		if (m) {
			if (m.attriutes & Constants.Monster.Undead) {
				dmg = 3;
			}
		}
		return [
			{ property: Constants.Mods.Damage_Magical, value: dmg},
			{ property: Constants.Mods.Skill_Horror, value: 1}
		];
	},
	type: Constants.ObjectType.CommonItem,
	tid: "CI_Cross",
	name: "Cross"
});
game.registerMultiObject(2, gen_Cross);


// Generated Skill "Fight"
var gen_Fight = game.createObject({
	description: "When you spend a Clue token to add to any <b>Fight check</b>, add one extra bonus die.",
	modifications: [ { property: Constants.Mods.Skill_Fight, value: "+1" }, { property: Constants.Mods.DieRoll_Fight, value: "+1" } ],
	type: Constants.ObjectType.Skill,
	tid: "SK_Fight",
	name: "Fight"
});
game.registerMultiObject(2, gen_Fight);

