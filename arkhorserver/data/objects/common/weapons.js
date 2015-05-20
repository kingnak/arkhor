
// .18 Derringer
var derringer = game.createObject({
	tid: "CI_.18_Derringer",
	name: ".18 Derringer",
	type: Constants.ObjectType.CommonItem,
	description: ".18 Derringer cannot be lost or stolen unless you choose to allow it.",
	hands: 1,
	price: 3,
	modifications: { property: Constants.Mods.Damage_Physical, value: 2}
});
game.registerMultiObject(2, derringer);

// .38 Revolver
var revolver = game.createObject({
	tid: "CI_.38_Revolver",
	name: ".38 Revolver",
	type: Constants.ObjectType.CommonItem,
	hands: 1,
	price: 4,
	modifications: { property: Constants.Mods.Damage_Physical, value: 3}
});
game.registerMultiObject(2, revolver);

// .45 Automatic
var automatic = game.createObject({
	tid: "CI_.45_Automatic",
	name: ".45 Automatic",
	type: Constants.ObjectType.CommonItem,
	hands: 1,
	price: 5,
	modifications: { property: Constants.Mods.Damage_Physical, value: 4}
});
game.registerMultiObject(2, automatic);

// Cavalry Saber
var saber = game.createObject({
	tid: "CI_Cavalry_Saber",
	name: "Cavalry Saber",
	type: Constants.ObjectType.CommonItem,
	hands: 1,
	price: 3,
	modifications: { property: Constants.Mods.Damage_Physical, value: 2}
});
game.registerMultiObject(2, saber);

// Cross
var cross = game.createObject({
	tid: "CI_Cross",
	name: "Cross",
	description: "+3 Combat Damage against <b>Undead</b>",
	type: Constants.ObjectType.CommonItem,
	hands: 1,
	price: 3,
	modifications: function() {
		debugger;
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
	}
});
game.registerMultiObject(2, cross);

// Knife
var knife = game.createObject({
	tid: "CI_Knife",
	name: "Knife",
	type: Constants.ObjectType.CommonItem,
	hands: 1,
	price: 2,
	modifications: { property: Constants.Mods.Damage_Physical, value: 1}
});
game.registerMultiObject(2, knife);

// Rifle
var rifle = game.createObject({
	tid: "CI_Rifle",
	name: "Rifle",
	type: Constants.ObjectType.CommonItem,
	hands: 2,
	price: 6,
	modifications: { property: Constants.Mods.Damage_Physical, value: 5}
});
game.registerMultiObject(2, rifle);

// Tommy Gun
var tommy = game.createObject({
	tid: "CI_Tommy_Gun",
	name: "Tommy Gun",
	type: Constants.ObjectType.CommonItem,
	hands: 2,
	price: 7,
	modifications: { property: Constants.Mods.Damage_Physical, value: 6}
});
game.registerMultiObject(2, tommy);
