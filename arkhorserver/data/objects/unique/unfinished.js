
// ALIEN STATE
var useStatue = game.quickOption({
	//id: "CI_USE_STATUE",
	name: "Use Alien Statue",
	costs: [[
		{ type: Constants.Costs.Movement, amount: 2},
		{ type: Constants.Costs.Sanity, amount: 1},
	]],
	phases: Constants.GamePhases.Movement,
	continueType: Constants.Option.CanContinue,
	chooseType: Constants.Option.Supplemental,
	activate: function() {
		this.exhaust();
		var res = game.context.genericDieRollTest("Use Statue", 1, 1);
		if (res) {
			// TODO: Let user decide
            //var obj = game.context.drawObject(Constants.ObjectType.Spell);
			//game.context.character.addToInventory(obj);
			game.context.character.addClue(3);
		} else {
			game.context.damageStamina(2);
		}
	}
});
var alienStatue = game.createObject({
	tid: "UI_Alien_Statue",
	name: "Alien Statue",
	type: Constants.ObjectType.UniqueItem,
	exhaustable: true,
	description: "<b>Movement:</b> Exhaust and spend 2 "+
		"movement points and 1 Sanity to roll a die. If "+
		"the die is a success, draw 1 Spell or gain 3 "+
		"Clue tokens. If it is a failure, lose 2 Stamina.",
	price: 5,
	optionIds: [useStatue.id]
});
game.registerMultiObject(1, alienStatue); 


// BLUE WATCHER OF THE PYRAMID
var useBlueWatcher = game.quickOption({
	//id: "CI_USE_BLUE_WATCHER",
	name: "Use Blue Watcher",
	costs: { type: Constants.Costs.Stamina, amount: 2},
	phases: Constants.GamePhases.Any,
	continueType: Constants.Option.CanContinue,
	chooseType: Constants.Option.Supplemental,
	activate: function() {
        this.discard();
		// TODO
	}
});
var blueWatcher = game.createObject({
	tid: "UI_Blue_Watcher_of_the_Pyramid",
	name: "Blue Watcher of the Pyramid",
	type: Constants.ObjectType.UniqueItem,
	exhaustable: true,
	description: "<b>Any Phase:</b> Lose 2 Stamina and "+
		"discard Blue Watcher of the Pyramid to "+
		"automatically succeed at a Combat check or a "+
		"Fight check or Lore check made to close a gate. "+
		"This cannot be used against an Ancient One."+
		"\nNot implemented",
	price: 4,
	optionIds: [useBlueWatcher.id]
});
game.registerMultiObject(1, blueWatcher); 

// DRAGON'S EYE
var useDragonsEye = game.quickOption({
	name: "Use Dragon's Eye",
	phases: Constants.GamePhases.Any,
	continueType: Constants.Option.CanContinue,
	chooseType: Constants.Option.Supplemental,
	activate: function() {
		this.owner.damageSanity(1);
        this.discard();
		// TODO
	}
});
var dragonsEye = game.createObject({
	tid: "UI_Dragons_Eye",
	name: "Dragon's Eye",
	type: Constants.ObjectType.UniqueItem,
	exhaustable: true,
	description: "<b>Any Phase:</b> Exhaust and lose 1 Sanity "+
		"after drawing a gate or location card to draw a "+
		"new card in its place."+
		"\nNot implemented",
	price: 6,
	optionIds: [useDragonsEye.id]
});
game.registerMultiObject(1, dragonsEye);

// FLUTE OF THE OUTER GODS
var useFlute = game.quickOption({
	name: "Use Flute of the Outer Gods",
	phases: Constants.GamePhases.Any,
	continueType: Constants.Option.CannotContinue,
	chooseType: Constants.Option.Supplemental,
	isAvailable: function() {
		// TODO: other way? when is it available then? (during fight?)
		return this.owner.field.hasMonsters();
	},
	activate: function() {
		debugger;
		this.owner.damageSanity(3);
		this.owner.damageStamina(3);
		// TODO: This won't work now
		var monsters = game.context.allMonsters;
		// Or: this.owner.field.allMonsters ??
		for (var i = 0; i < monsters.length; i++) {
			// Well, defeat means "defeated by"
			monsters[i].defeat(this.owner);
		}
        this.discard();
	}
});
var flute = game.createObject({
	tid: "UI_Flute_of_the_Outer_Gods",
	name: "Flute of the Outer Gods",
	type: Constants.ObjectType.UniqueItem,
	description: "<b>Any Phase:</b> Lose 3 Sanity and "+
		"3 Stamina and discard Flute of the Outer Gods "+
		"before making a Combat check to defeat all monsters "+
		"in your current area. This does not affect Ancient Ones.",
	price: 8,
	optionIds: [useFlute.id]
});
game.registerMultiObject(1, flute);

// Holy Water
var holyWater = game.createObject({
	tid: "UI_Holy_Water",
	name: "Holy Water",
	description: "Discard after use",
	type: Constants.ObjectType.UniqueItem,
	hands: 2,
	price: 4,
	modifications: { property: Constants.Mods.Damage_Magical, value: 6}
	// TODO: discard after use!
});
game.registerMultiObject(4, holyWater);


////////////////////////
/* TODOS:
 * Enchanted Jewelry
 * Gate Box
 * Lamp of Alhazred
 * Nameless Cults
 * Necronomicon
 * Obsidian Statue
 * Pallid Mask
 * Powder of Ibn-Ghazi
 * Ruby of R'lyeh
 * Silver Key
 * Sword of Glory
 * The King in Yellow
 * Warding Statue 
 */
