 
// Dread Curse of Azathoth
var curseAzathoth = game.createObject({
	tid: "SP_Dread_Curse_of_Azathoth",
	name: "Dread Curse of Azathoth",
	type: Constants.ObjectType.Spell,
	exhaustable: true,
	description: "<b>Any Phase:</b> Cast and exhaust to "+
		"gain +9 to Combat checks until the end of this combat.",
	modifications: { property: Constants.Mods.Damage_Magical, value: 9 },
	hands: 2,
	castCost: 2,
	castAdjustment: -2
});
game.registerMultiObject(4, curseAzathoth);