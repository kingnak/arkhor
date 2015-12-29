 
// ELDER SIGN
var useElderSign = game.quickOption({
	name: "Use Elder Sign",
	phases: Constants.GamePhases.Any,
	continueType: Constants.Option.CannotContinue,
	chooseType: Constants.Option.Supplemental,
	isAvailable: function() {
        return this.owner.canCloseGate();
	},
	activate: function() {
		debugger;
        game.context.gate.seal(this.owner);
		this.owner.damageStamina(1);
		this.owner.damageSanity(1);
		// TODO: remove token from doom track
        //this.owner.removeFromInventory(this);
        this.removeFromGame();
	}
});
var elderSign = game.createObject({
	tid: "UI_Elder_Sign",
	name: "Elder Sign",
	type: Constants.ObjectType.UniqueItem,
	exhaustable: true,
	description: "<b>Any Phase:</b> When sealing a gate, "+
		"lose 1 Stamina and 1 Sanity and return this card "+
		"to the box. You do not need to make a skill check "+
		"or spend any Clue tokens to seal the gate. In "+
		"addition, remove one doom token from the Ancient "+
		"One's doom track.",
	price: 5,
	optionIds: [useElderSign.id]
});
game.registerMultiObject(4, elderSign);


// HEALING STONE
var healingStone = game.createObject({
	tid: "UI_Healing_Stone",
	name: "Healing Stone",
	type: Constants.ObjectType.UniqueItem,
	description: "<b>Upkeep:</b> You may gain 1 Stamina or 1 Sanity.\n"+
		"Discard Healing Stone if the Ancient One awakens.",
	price: 8,
    exhaustable: true,
	optionIds: [
		game.quickOption({
			name: "Heal Stamina (Healing Stone)",
			phases: Constants.GamePhases.Upkeep,
			continueType: Constants.Option.CanContinue,
			chooseType: Constants.Option.Supplemental,
			activate: function() {
				this.owner.addStamina(1);
				this.exhaust();
			}
		}).id,
		game.quickOption({
			name: "Heal Sanity (Healing Stone)",
			phases: Constants.GamePhases.Upkeep,
			continueType: Constants.Option.CanContinue,
			chooseType: Constants.Option.Supplemental,
			activate: function() {
				this.owner.addSanity(1);
				this.exhaust();
			}
		}).id
	]
	// TODO: Discard when ancient one awakens
});
game.registerMultiObject(1, healingStone);

