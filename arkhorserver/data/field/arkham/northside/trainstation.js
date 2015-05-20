// Encounters
var train_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.NS_TrainStation,
	description: "A stranger in a turban steps off the "+
		"Boston local train with a crazed look on his face. "+
		"Make a <b>Luck (-1) check</b>. If you pass, the "+
		"man pulls a strange object from beneath his cloak "+
		"and gives it to you. Draw 1 Unique Item. If you "+
		"fail, he pulls a poisoned blade out of his cloak "+
		"and stabs you. Roll a die and lose that much Stamina.",
	options: [
		game.quickOption({
			name: "Sell Story",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Let him draw", Constants.Skill.Luck, -1, 1);
				if (res) {
					var ob = game.drawObject(Constants.ObjectType.UniqueItem);
					game.context.character.addToInventory(ob);
				} else {
					var dmg = game.context.genericDieRollSum("Damage by poisoned blade", 1);
					game.context.character.damageStamina(dmg);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(train_enc1);

var train_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.NS_TrainStation,
	description: "A well-dressed man is standing on the "+
		"platform. He turns and greets you by name. "+
		"Although he seems oddly familiar, you don't "+
		"remember ever meeting him before. Then he steps off "+
		"the platform into the path of a speeding train. "+
		"Make a <b>Speed (-1) check</b>. If you pass, he "+
		"vanishes as you leap right through him. On the "+
		"ground, you find yourself clutching a scrap of "+
		"paper. Gain 1 Spell. If you fail, he is "+
		"obliterated before your eyes. Roll a die and "+
		"lose that much Sanity.",
	options: [
		game.quickOption({
			name: "Catch stranger",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Catch stranger", Constants.Skill.Speed, -1, 1);
				if (res) {
					var ob = game.drawObject(Constants.ObjectType.Spell);
					game.context.character.addToInventory(ob);
				} else {
					var dmg = game.context.genericDieRollSum("Damage by seeing him killed", 1);
					game.context.character.damageSanity(dmg);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(train_enc2);
