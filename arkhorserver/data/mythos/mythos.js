
game.registerMythosCard(game.createMythosCard({
	name: "TestMythos",
	type: Constants.Mythos.Headline,
  description: "<b>Ongoing Effect: </b>Return one Ally from the Ally deck to the box "+
      "at random at the end of every <b>Mythos Phase</b> while this card is in play "+
      "(beginning the turn after it entered play). The Southside Strangler has struck again!\n\n"+
      "<b>Pass: </b>If a single player discards 5 Clue tokens while in Ma's Boarding House during "+
      "the <b>Arkham Encounters Phase</b>, return this card to the box. Each player receives a $5 reward from the police.\n\n"+
      "<b>Fail: </b>If there are no more Allies to return to the box at the end of the "+
      "<b>Mythos Phase</b>, return this card to the box. Each player must lower either their "+
      "maximum Sanity or Stamina (their choice) by 1 for the rest of the game.",

	clueField: Constants.Fields.DT_ArkhamAsylum,
    //gateField: Constants.Fields.DT_ArkhamAsylum,
	moveBlack: Constants.Dimension.Square,
    moveWhite: Constants.Dimension.Triangle,

    executeHeadline: function() {}
}));
