 
// ANCIENT TABLET
var readTablet = game.quickOption({
	//id: "CI_READ_TABLET",
	name: "Read Ancient Tablet",
	costs: { type: Constants.Costs.Movement, amount: 3},
	phases: Constants.GamePhases.Movement,
	continueType: Constants.Option.CanContinue,
	chooseType: Constants.Option.Supplemental,
	activate: function() {
		var res = game.context.genericDieRollCount("Read Tablet", 2);
		for (var i = 0; i < res; i++) {
            var obj = game.context.drawObject(Constants.ObjectType.Spell);
			game.context.character.addToInventory(obj);
		}
		for (var i = 0; i < 2-res; i++) {
			game.context.addClue(2);
		}
        this.discard();
	}
});
var ancientTablet = game.createObject({
	tid: "UI_Ancient_Tablet",
	name: "Ancient Tablet",
	type: Constants.ObjectType.UniqueItem,
	exhaustable: true,
	description: "<b>Movement:</b> Spend 3 movement points "+
		"and discard Ancient Tablet to roll 2 dice. For "+
		"every success rolled, draw 1 Spell. For every "+
		"failure rolled, gain 2 Clue tokens.",
	price: 8,
	optionIds: [readTablet.id]
});
game.registerMultiObject(1, ancientTablet); 

// BOOK OF DZYAN
var readDzyan = game.quickOption({
	//id: "CI_READ_BOOK_OF_DZYAN",
    name: "Read Book of Dzyan",
	costs: { type: Constants.Costs.Movement, amount: 2},
	phases: Constants.GamePhases.Movement,
	continueType: Constants.Option.CanContinue,
	chooseType: Constants.Option.Supplemental,
	activate: function() {
		this.exhaust();
		var res = game.context.skillTest("Read Book of Dzyan", Constants.Skills.Lore, -1, 1);
		if (res) {
            var ob = game.context.drawObject(Constants.ObjectType.Spell);
			this.owner.addToInventory(ob);
			this.owner.damageSanity(1);
			// Place tokens
			this.tokens++;
			if (this.tokens >= 2) {
                this.discard();
			}
		}
	}
});
var dzyan = game.createObject({
	tid: "UI_Book_of_Dzyan",
	name: "Book of Dzyan",
	type: Constants.ObjectType.UniqueItem,
	exhaustable: true,
	description: "<b>Movement:</b> Exhaust and spend 2 "+
		"movement points to make a <b>Lore (-1) check</b>. "+
		"If you pass, draw 1 Spell, lose 1 Sanity, and "+
		"put 1 Stamina token from the bank on Book of "+
		"Dzyan. If there are 2 Stamina tokens on it, "+
		"discard Book of Dzyan. If you fail, nothing happens.",
	price: 3,
	optionIds: [readDzyan.id],
	onAddToInventory: function(c) {
		this.tokens = 0;
    },
    properties: {
        tokens: { display: "Tokens on Book", value: 0, onDisplay: function() { return this.tokens + " / 2"; } }
    }
});
game.registerMultiObject(1, dzyan); 

// CABALA OF SABOTH
var readCabala = game.quickOption({
	name: "Read Cabala of Saboth",
	costs: { type: Constants.Costs.Movement, amount: 2},
	phases: Constants.GamePhases.Movement,
	continueType: Constants.Option.CanContinue,
	chooseType: Constants.Option.Supplemental,
	activate: function() {
		this.exhaust();
		var res = game.context.skillTest("Read Cabala of Saboth", Constants.Skills.Lore, -2, 1);
		if (res) {
            var ob = game.context.drawObject(Constants.ObjectType.Skill);
			this.owner.addToInventory(ob);
            this.discard();
		}
	}
});
var cabala = game.createObject({
	tid: "UI_Cabala_of_Saboth",
	name: "Cabala of Saboth",
	type: Constants.ObjectType.UniqueItem,
	exhaustable: true,
	description: "<b>Movement:</b> Exhaust and spend 2 "+
		"movement points to make a <b>Lore (-2) check</b>. "+
		"If you pass, draw 1 Skill and discard Cabala of "+
		"Saboth. If you fail, nothing happens.",
	price: 5,
	optionIds: [readCabala.id]
});
game.registerMultiObject(2, cabala);

// CULTES DES GOULES
var readCultes = game.quickOption({
	name: "Read Cultes des Goules",
	costs: { type: Constants.Costs.Movement, amount: 2},
	phases: Constants.GamePhases.Movement,
	continueType: Constants.Option.CanContinue,
	chooseType: Constants.Option.Supplemental,
	activate: function() {
		this.exhaust();
		var res = game.context.skillTest("Read Cultes des Goules", Constants.Skills.Lore, -2, 1);
		if (res) {
            var ob = game.context.drawObject(Constants.ObjectType.Spell);
			this.owner.addToInventory(ob);
			this.owner.addClue(1);
			this.owner.damageSanity(2);
            this.discard();
		}
	}
});
var cultes = game.createObject({
	tid: "UI_Cultes_des_Goules",
	name: "Cultes des Goules",
	type: Constants.ObjectType.UniqueItem,
	exhaustable: true,
	description: "<b>Movement:</b> Exhaust and spend 2 "+
		"movement points to make a <b>Lore (-2) check</b>. "+
		"If you pass, draw 1 Spell and gain 1 Clue token, "+
		"but lose 2 Sanity and discard Cultes Des Goules. "+
		"If you fail, nothing happens.",
	price: 3,
	optionIds: [readCultes.id]
});
game.registerMultiObject(2, cultes);
