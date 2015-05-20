 

// Find Gate
var findGate = game.createObject({
	tid: "SP_Find_Gate",
	name: "Find Gate",
	type: Constants.ObjectType.Spell,
	exhaustable: true,
	description: "<b>Movement: </b> Cast and exhaust to "+
		"immediately return to Arkham from an Other World.",
    castCost: 1,
	castAdjustment: -1,
    cast: {
        cast: function() {
            this.owner.returnToArkham();
        },
        phases: Constants.GamePhases.Movement,
        isAvailable: function() {
            return game.context.character.field.type == Constants.FieldType.OtherWorld;
        }
    }
});
game.registerMultiObject(4, findGate);
