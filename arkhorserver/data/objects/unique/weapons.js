 
// Enchanted Blade
var encBlade = game.createObject({
	tid: "UI_Enchanted_Blade",
	name: "Enchanted Blade",
	type: Constants.ObjectType.UniqueItem,
	hands: 1,
	price: 6,
	modifications: { property: Constants.Mods.Damage_Magical, value: 4}
});
game.registerMultiObject(2, encBlade);

 // Enchanted Knife
var encKnife = game.createObject({
	tid: "UI_Enchanted_Knife",
	name: "Enchanted Knife",
	type: Constants.ObjectType.UniqueItem,
	hands: 1,
	price: 5,
	modifications: { property: Constants.Mods.Damage_Magical, value: 3}
});
game.registerMultiObject(2, encKnife);
