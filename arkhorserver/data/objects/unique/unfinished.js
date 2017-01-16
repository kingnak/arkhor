
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


////////////////////////
/* TODOS:
 * Gate Box x 1: $4 Any Phase: When you return to Arkham from an Other World, you can return to any location with an open gate, not just those leading to the Other World you were in.
 * Obsidian Statue x 1: $4 Any Phase: Discard Obsidian Statue to cancel all Stamina or Sanity loss being dealt to you from one source. 
 * Silver Key x 1: $4 Any Phase: Put 1 Stamina token from the bank on Silver Key before making an Evade check to automatically pass it. Discard Silver Key after using it if there are 3 Stamina tokens on it. 
 * Warding Statue x 1: $6 Any Phase: Discard Warding Statue after failing a Combat check to reduce the monster's combat damage to 0 Stamina. This can also be used to cancel an Ancient One's entire attack for 1 turn. 
 */
