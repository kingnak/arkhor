
// ANCIENT TOME
var readTome = game.quickOption({
	//id: "CI_READ_TOME",
	name: "Read Ancient Tome",
	costs: { type: Constants.Costs.Movement, amount: 2},
	phases: Constants.GamePhases.Movement,
	continueType: Constants.Option.CanContinue,
	chooseType: Constants.Option.Supplemental,
	activate: function() {
		this.exhaust();
		var res = game.context.skillTest("Read Tome", Constants.Skills.Lore, -1, 1);
		if (res) {
			this.returnToDeck();
            var obj = game.context.drawObject(Constants.ObjectType.CommonItem);
			game.context.character.addToInventory(obj);
		}
	}
});
var tome = game.createObject({
	tid: "CI_Ancient_Tome",
	name: "Ancient Tome",
	type: Constants.ObjectType.CommonItem,
	exhaustable: true,
	description: "<b>Movement:</b> Exhaust and spend 2 movement points to make a <b>Lore (-1) check</b>. "+
		"If you pass, draw 1 Spell and discard Ancient Tome. If you fail, nothing happens.",
	price: 4,
	optionIds: [readTome.id]
});
game.registerMultiObject(2, tome);

// Dark Cloak
var cloak = game.createObject({
	tid: "CI_Dark_Cloak",
	name: "Dark Cloak",
	type: Constants.ObjectType.CommonItem,
	price: 2,
	modifications: { property: Constants.Mods.Skill_Evade, value: 1}
});
game.registerMultiObject(2, cloak);

// Food
var food = game.createObject({
	tid: "CI_Food",
	name: "Food",
    description: "<b>Any:</b> Discard Food to reduce any Stamina loss by 1.",
	type: Constants.ObjectType.CommonItem,
	price: 1,
	options: game.quickOption({
		name: "Eat Food",
		phases: Constants.GamePhases.Any,
		activate: function() {
			this.owner.preventDamageStamina(1);
			this.returnToDeck();
		}
	}).id
});
game.registerMultiObject(2, food);

// Lantern
var lantern = game.createObject({
	tid: "CI_Lantern",
	name: "Lantern",
	type: Constants.ObjectType.CommonItem,
	price: 3,
	modifications: { property: Constants.Mods.Skill_Luck, value: 1}
	/*
	 * NOTE: This is corrected lantern from Dunwich Horror.
	 * Original lantern had:
	 * Any Phase: Exhaust to re-roll 1 die after making a Luck check.
	 * And cost $2
	 */
});
game.registerMultiObject(2, lantern);

// Map of Arkham
var map = game.createObject({
	tid: "CI_Map_of_Arkham",
	name: "Map of Arkham",
	exhaustable: true,
    description: "<b>Movement:</b> Exhaust to get 1 extra movement point.",
	type: Constants.ObjectType.CommonItem,
	price: 2,
	options: game.quickOption({
		name: "Read Map",
        phases: Constants.GamePhases.Movement,
		activate: function() {
			this.exhaust();
			this.owner.addMovementPoint(1);
		}
	}).id
});
game.registerMultiObject(2, map);

// Motorcycle
var motorcycle = game.createObject({
	tid: "CI_Motorcycle",
	name: "Motorcycle",
	exhaustable: true,
    description: "<b>Movement:</b> Exhaust to get 2 extra movement point.",
	type: Constants.ObjectType.CommonItem,
	price: 4,
	options: game.quickOption({
		name: "Read Map",
        phases: Constants.GamePhases.Movement,
		activate: function() {
			this.exhaust();
			this.owner.addMovementPoint(2);
		}
	}).id
});
game.registerMultiObject(2, motorcycle);

// OLD JOURNAL
var readJournal = game.quickOption({
	//id: "CI_OLD_JOURNAL",
	name: "Read Old Journal",
	costs: { type: Constants.Costs.Movement, amount: 1},
	phases: Constants.GamePhases.Movement,
	continueType: Constants.Option.CanContinue,
	chooseType: Constants.Option.Supplemental,
	activate: function() {
		this.exhaust();
		var res = game.context.skillTest("Read Old Journal", Constants.Skills.Lore, -1, 1);
		if (res) {
			this.returnToDeck();
			game.context.character.addClue(3);
		}
	}
});
var journal = game.createObject({
    tid: "CI_Old_Journal",
    name: "Old Journal",
	type: Constants.ObjectType.CommonItem,
	exhaustable: true,
	description: "<b>Movement:</b> Exhaust and spend 1 movement point to make a <b>Lore (-1) check</b>. "+
		"If you pass, gain 3 Clue tokens and discard Old Journal. If you fail, nothing happens.",
	price: 1,
	optionIds: [readJournal.id]
});
game.registerMultiObject(2, journal);

// Research Materials
var resMat = game.createObject({
	tid: "CI_Research_Materials",
	name: "Research Materials",
	exhaustable: true,
    description: "<b>Any:</b> Discard Research Materials instead of spending 1 Clue token.",
	type: Constants.ObjectType.CommonItem,
	price: 1,
	options: game.quickOption({
		name: "Use Research Materials",
		phases: Constants.GamePhases.Any,
		activate: function() {
			// This is not 100% correct, but good enough...
			this.owner.addClue(1);
			this.returnToDeck();
		}
	}).id
});
game.registerMultiObject(2, resMat);

// Whiskey
var whiskey = game.createObject({
	tid: "CI_Whiskey",
	name: "Whiskey",
    description: "<b>Any:</b> Discard Whiskey to reduce any Sanity loss by 1.",
	type: Constants.ObjectType.CommonItem,
	price: 1,
	options: game.quickOption({
		name: "Drink Whiskey",
		phases: Constants.GamePhases.Any,
		activate: function() {
			this.owner.preventDamageSanity(1);
			this.returnToDeck();
		}
	}).id
});
game.registerMultiObject(2, whiskey);
