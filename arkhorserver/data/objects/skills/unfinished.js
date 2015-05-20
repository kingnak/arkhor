 
// Bravery
var bravery = game.createObject({
	tid: "SK_Bravery",
	name: "Bravery",
	description: "<b>Any Phase:</b> Exhaust to re-roll a <b>Horror check</b>.",
	type: Constants.ObjectType.Skill,
	// TODO Reroll option
});
game.registerMultiObject(2, bravery);

// Expert Occultist
var occultist = game.createObject({
	tid: "SK_Expert_Occultist",
	name: "Expert Occultist",
	description: "<b>Any Phase:</b> Exhaust to re-roll a <b>Spell check</b>.",
	type: Constants.ObjectType.Skill,
	// TODO Reroll option
});
game.registerMultiObject(2, occultist);

// Marksman
var marksman = game.createObject({
	tid: "SK_Marksman",
	name: "Marksman",
	description: "<b>Any Phase:</b> Exhaust to re-roll a <b>Combat check</b>.",
	type: Constants.ObjectType.Skill,
	// TODO Reroll option
});
game.registerMultiObject(2, marksman);

// Stealth
var stealth = game.createObject({
	tid: "SK_Stealth",
	name: "Stealth",
	description: "<b>Any Phase:</b> Exhaust to re-roll a <b>Evade check</b>.",
	type: Constants.ObjectType.Skill,
	// TODO Reroll option
});
game.registerMultiObject(2, stealth);
