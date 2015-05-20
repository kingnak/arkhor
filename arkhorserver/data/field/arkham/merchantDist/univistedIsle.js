// Encounters
var isle_enc1 = game.createArkhamEncounter({
	field: Constants.Fields.MD_UnvisitedIsle,
	description: "A group of hooded cultists are having "+
		"a meeting among the standing stones on the island. "+
		"Pass a <b>Sneak (-1) check</b> to overhear some of "+
		"what they have to say. Gain 2 Clue tokens.",
	options: [
		game.quickOption({
			name: "Eavesdrop",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				var res = game.context.skillTest("Eavesdrop", Constants.Skills.Sneak, -1, 1);
				if (res) {
					game.context.character.addClue(2);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(isle_enc1);

var isle_enc2 = game.createArkhamEncounter({
	field: Constants.Fields.MD_UnvisitedIsle,
	description: "A silent man brushes past you on the "+
		"trail. Your arm goes numb with cold from the brief "+
		"contact, and you whirl around to look at him, but "+
		"he has disappeared. Lose 1 Stamina and pass a "+
		"<b>Will (-1) check</b> or lose 1 Sanity as well.",
	options: [
		game.quickOption({
			name: "Resist",
			phases: Constants.GamePhases.ArkhamEncountery,
			activate: function() {
				game.context.character.damageStamina(1);
				var res = game.context.skillTest("Resist", Constants.Skills.Will, -1, 1);
				if (!res) {
					game.context.character.damageSanity(1);
				}
			}
		}).id
	]
});
game.registerArkhamEncounter(isle_enc2);
