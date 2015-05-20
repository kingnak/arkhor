// Anna Kaslow
var annaKaslow = game.createObject({
	tid: "AL_Anna_Kaslow",
	name: "Anna Kaslow",
	description: "Gain 2 Clue tokens when Anna Kaslow joins you.",
	type: Constants.ObjectType.Ally,
	modifications: { property: Constants.Mods.Skill_Luck, value: 2},
	onAddToInventory: function(c) {
		c.addClue(2);
	}
});
game.registerSingleObject(annaKaslow);
 
// Duke
var duke = game.createObject({
	tid: "AL_Duke",
	name: "Duke",
	description: "Discard to immediately restore your Sanity to its maximum.",
	type: Constants.ObjectType.Ally,
	modifications: { property: Constants.Mods.Prop_MaxSanity, value: 1},
	options: [
		game.quickOption({
			name: "Abandon Duke",
			phases: Constants.GamePhases.Any,
			activate: function() {
				owner.restoreSanity();
                this.discard();
			}
		}).id
	]
});
game.registerSingleObject(duke);
 
// Ruby Standish
var rubyStandish = game.createObject({
	tid: "AL_Ruby_Standish",
	name: "Ruby Standish",
	description: "Draw 1 Unique item when Ruby Standish joins you.",
	type: Constants.ObjectType.Ally,
	modifications: { property: Constants.Mods.Skill_Sneak, value: 2},
	onAddToInventory: function(c) {
        var ob = game.context.drawObject(Constants.ObjectType.UniqueItem);
		c.addToInventory(ob);
	}
});
game.registerSingleObject(rubyStandish);

// Ryan Dean
var ryanDean = game.createObject({
	tid: "AL_Ryan_Dean",
	name: "Ryan Dean",
	description: "Draw 1 Common item when Ryan Dean joins you.",
	type: Constants.ObjectType.Ally,
	modifications: [
		{ property: Constants.Mods.Skill_Sneak, value: 1},
		{ property: Constants.Mods.Skill_Will, value: 1}
	],
	onAddToInventory: function(c) {
        var ob = game.context.drawObject(Constants.ObjectType.CommonItem);
		c.addToInventory(ob);
	}
});
game.registerSingleObject(ryanDean);

// Sir William Brinton
var sirBrinton = game.createObject({
	tid: "AL_Sir_William_Brinton",
	name: "Sir William Brinton",
	description: "Discard to immediately restore your Stamina to its maximum.",
	type: Constants.ObjectType.Ally,
	modifications: { property: Constants.Mods.Prop_MaxStamina, value: 1},
	options: [
		game.quickOption({
			name: "Abandon Sir William Brinton",
			phases: Constants.GamePhases.Any,
			activate: function() {
				owner.restoreStamina();
                this.discard();
			}
		}).id
	]
});
game.registerSingleObject(sirBrinton);

// Thomas F. Malone
var thomMalone = game.createObject({
	tid: "AL_Thomas_F_Malone",
	name: "Thomas F. Malone",
	description: "Draw 1 Spell when Thomas F. Malone joins you.",
	type: Constants.ObjectType.Ally,
	modifications: [
		{ property: Constants.Mods.Skill_Fight, value: 1},
		{ property: Constants.Mods.Skill_Lore, value: 1}
	],
	onAddToInventory: function(c) {
        var ob = game.context.drawObject(Constants.ObjectType.Spell);
		c.addToInventory(ob);
	}
});
game.registerSingleObject(thomMalone);
