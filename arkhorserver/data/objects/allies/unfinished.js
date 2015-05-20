// Eric Colt
var ericColt = game.createObject({
	tid: "AL_Eric_Colt",
	name: "Eric Colt",
	description: "You take no Sanity loss from the Nightmarish ability."+
		"\nNot implemented",
	type: Constants.ObjectType.Ally,
	modifications: { property: Constants.Mods.Skill_Speed, value: 2}
	// TODO: Prevent Nightmarish damage
});
game.registerSingleObject(ericColt);

// John Legrasse
var johnLegrasse = game.createObject({
	tid: "AL_John_Legrasse",
	name: "John Legrasse",
	description: "You can claim monsters with the Endless ability as trophies."+
		"\nNot implemented",
	type: Constants.ObjectType.Ally,
	modifications: { property: Constants.Mods.Skill_Will, value: 2}
	// TODO: Ignore Endless
});
game.registerSingleObject(johnLegrasse);

// Professor Armitage
var profArmitage = game.createObject({
	tid: "AL_Professor_Armitage",
	name: "Professor Armitage",
	description: "Your attacks are not affected by Magical Resistance."+
		"\nNot implemented",
	type: Constants.ObjectType.Ally,
	modifications: { property: Constants.Mods.Skill_Lore, value: 2}
	// TODO: Ignore Magical Resistance
});
game.registerSingleObject(profArmitage);

// Richard Upton Pickman
var rUPickman = game.createObject({
	tid: "AL_Richard_Upton_Pickman",
	name: "Richard Upton Pickman",
	description: "Your attacks are not affected by Physical Resistance."+
		"\nNot implemented",
	type: Constants.ObjectType.Ally,
	modifications: [
		{ property: Constants.Mods.Skill_Luck, value: 1},
		{ property: Constants.Mods.Skill_Speed, value: 1}
	]
	// TODO: Ignore Physical Resistance
});
game.registerSingleObject(rUPickman);

// Tom "Mountain" Murphy
var ericColt = game.createObject({
	tid: "AL_Tom_Mountain_Murphy",
	name: "Tom \"Mountain\" Murphy",
	description: "You take no Stamina loss from the Overwhelming ability."+
		"\nNot implemented",
	type: Constants.ObjectType.Ally,
	modifications: { property: Constants.Mods.Skill_Fight, value: 2}
	// TODO: Prevent Nightmarish damage
});
game.registerSingleObject(ericColt);
