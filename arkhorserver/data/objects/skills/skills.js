 
// Fight
var fight = game.createObject({
	tid: "SK_Fight",
	name: "Fight",
	description: "When you spend a Clue token to add to any <b>Fight check</b>, add one extra bonus die.",
	type: Constants.ObjectType.Skill,
	modifications: [
		{ property: Constants.Mods.Skill_Fight, value: 1 },
		{ property: Constants.Mods.DieRoll_Fight, value: 1 }
	]
});
game.registerMultiObject(2, fight);

// Lore
var lore = game.createObject({
	tid: "SK_Lore",
	name: "Lore",
	description: "When you spend a Clue token to add to any <b>Lore check</b>, add one extra bonus die.",
	type: Constants.ObjectType.Skill,
	modifications: [
		{ property: Constants.Mods.Skill_Lore, value: 1 },
		{ property: Constants.Mods.DieRoll_Lore, value: 1 }
	]
});
game.registerMultiObject(2, lore);

// Luck
var luck = game.createObject({
    tid: "SK_Luck",
    name: "Luck",
    description: "When you spend a Clue token to add to any <b>Luck check</b>, add one extra bonus die.",
    type: Constants.ObjectType.Skill,
    modifications: [
        { property: Constants.Mods.Skill_Luck, value: 1 },
        { property: Constants.Mods.DieRoll_Luck, value: 1 }
    ]
});
game.registerMultiObject(2, luck);

// Sneak
var sneak = game.createObject({
    tid: "SK_Sneak",
    name: "Sneak",
    description: "When you spend a Clue token to add to any <b>Sneak check</b>, add one extra bonus die.",
    type: Constants.ObjectType.Skill,
    modifications: [
        { property: Constants.Mods.Skill_Sneak, value: 1 },
        { property: Constants.Mods.DieRoll_Sneak, value: 1 }
    ]
});
game.registerMultiObject(2, sneak);

// Speed
var speed = game.createObject({
    tid: "SK_Speed",
    name: "Speed",
    description: "When you spend a Clue token to add to any <b>Speed check</b>, add one extra bonus die.",
    type: Constants.ObjectType.Skill,
    modifications: [
        { property: Constants.Mods.Skill_Speed, value: 1 },
        { property: Constants.Mods.DieRoll_Speed, value: 1 }
    ]
});
game.registerMultiObject(2, speed);

// Will
var will = game.createObject({
    tid: "SK_Will",
    name: "Will",
    description: "When you spend a Clue token to add to any <b>Will check</b>, add one extra bonus die.",
    type: Constants.ObjectType.Skill,
    modifications: [
        { property: Constants.Mods.Skill_Will, value: 1 },
        { property: Constants.Mods.DieRoll_Will, value: 1 }
    ]
});
game.registerMultiObject(2, will);

// Bravery
var bravery = game.createObject({
    tid: "SK_Bravery",
    name: "Bravery",
    description: "<b>Any Phase:</b> Exhaust to re-roll a <b>Horror check</b>.",
    type: Constants.ObjectType.Skill,
    optionIds: [game.getDieRollOption({type: Constants.Reroll.All, skills: [Constants.Skills.Horror]})],
});
game.registerMultiObject(2, bravery);

// Expert Occultist
var occultist = game.createObject({
    tid: "SK_Expert_Occultist",
    name: "Expert Occultist",
    description: "<b>Any Phase:</b> Exhaust to re-roll a <b>Spell check</b>.",
    type: Constants.ObjectType.Skill,
    optionIds: [game.getDieRollOption({type: Constants.Reroll.All, skills: [Constants.Skills.Spell]})],
});
game.registerMultiObject(2, occultist);

// Marksman
var marksman = game.createObject({
    tid: "SK_Marksman",
    name: "Marksman",
    description: "<b>Any Phase:</b> Exhaust to re-roll a <b>Combat check</b>.",
    type: Constants.ObjectType.Skill,
    optionIds: [game.getDieRollOption({type: Constants.Reroll.All, skills: [Constants.Skills.Combat]})],
});
game.registerMultiObject(2, marksman);

// Stealth
var stealth = game.createObject({
    tid: "SK_Stealth",
    name: "Stealth",
    description: "<b>Any Phase:</b> Exhaust to re-roll a <b>Evade check</b>.",
    type: Constants.ObjectType.Skill,
    optionIds: [game.getDieRollOption({type: Constants.Reroll.All, skills: [Constants.Skills.Evade]})],
});
game.registerMultiObject(2, stealth);
