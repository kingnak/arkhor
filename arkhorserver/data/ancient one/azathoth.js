game.registerAncientOne(game.createAncientOne({
	id: "AO_AZATHOTH",
	name: "Azathoth",
	combatAdjustment: 0,
	worshippersText: "Since Azathoth promises nothing except destruction, "+
		"only the insane worship him. However, their fanaticism gives them "+
		"strength. Maniacs have their toughness increased by 1.",
	powerText: "Absolute Destruction - If Azathoth awakens, the game is over and the investigators lose.",
	attackText: "The end is here! Azathoth destroys the world.",
	doomTrack: 14,
	monsterModifications: [
		{ id: "MO_MANIAC", mod: {property: Constants.Mods.Monster_Toughness, value: 1} }
	],
	onAwake: function() {
		// TODO: Lose game
	}
}));
