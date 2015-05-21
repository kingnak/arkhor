var stranglerFieldOption = game.quickOption({
    costs: { type: Constants.Costs.Clue, amount: 5},
    name: "Catch Southside Strangler",
    description: "Catch the Southside strangler to pass the Rumor",
    phases: Constants.GamePhases.ArkhamEncountery,
    continueType: Constants.Option.CannotContinue,
    activate: function() {
        game.context.rumor.pass();
    }
});

game.registerMultiObject(8, game.createObject({
    tid: "OBJ_STRANGLER_DECREASE_STAMINA",
    name: "Southside Strangler Curse",
    description: "You failed the Southside strangler rumor.",
    type: Constants.ObjectType.Special,
    modifications: { property: Constants.Mods.Prop_MaxStamina, value: -1 }
}));

game.registerMultiObject(8, game.createObject({
    tid: "OBJ_STRANGLER_DECREASE_SANITY",
    name: "Southside Strangler Curse",
    description: "You failed the Southside strangler rumor.",
    type: Constants.ObjectType.Special,
    modifications: { property: Constants.Mods.Prop_MaxSanity, value: -1 }
}));

game.registerMythosCard(game.createMythosCard({
    name: "The Southside Strangler Strikes",
    type: Constants.Mythos.Rumor,
    description: "<b>Ongoing Effect: </b>Return one Ally from the Ally deck to the box "+
        "at random at the end of every <b>Mythos Phase</b> while this card is in play "+
        "(beginning the turn after it entered play). The Southside Strangler has struck again!\n\n"+
        "<b>Pass: </b>If a single player discards 5 Clue tokens while in Ma's Boarding House during "+
        "the <b>Arkham Encounters Phase</b>, return this card to the box. Each player receives a $5 reward from the police.\n\n"+
        "<b>Fail: </b>If there are no more Allies to return to the box at the end of the "+
        "<b>Mythos Phase</b>, return this card to the box. Each player must lower either their "+
        "maximum Sanity or Stamina (their choice) by 1 for the rest of the game.",

    shortDescription: "<b>Pass:</b> Spend 5 Clues at Ma's Boarding House\n"+
                      "<b>Fail:</b> No Allies are remaining",

    //clueField: Constants.Fields.DT_ArkhamAsylum,
    //gateField: Constants.Fields.DT_IndependenceSquare,
    //moveBlack: Constants.Dimension.Square,
    //moveWhite: Constants.Dimension.Triangle,

    rumorField: Constants.Fields.SS_MasBoardingHouse,
    rumorFieldOptionId: stranglerFieldOption.id,
    setupRumor: function() {
        this.allyCount = game.cardsOnDeck(Constants.ObjectType.Ally);
    },
    teardownRumor: function() {
    },
    onMythos: function() {
        var ally = game.drawSingleObject(Constants.ObjectType.Ally);
        this.allyCount = game.cardsOnDeck(Constants.ObjectType.Ally);
        if (!ally) {
            // No more allies => Fail
            this.fail();
        } else {
            ally.removeFromGame();
        }
    },
    onPass: function() {
        debugger;
        var list = game.context.allCharacters;
        for (var i = 0; i < list.length; ++i) {
            var c = list[i];
            c.addMoney(5);
        }
        this.removeFromGame();
    },
    onFail: function() {
        debugger;
        var sel = game.context.selectChoice("Select item to reduce Maximum value", ["Stamina", "Sanity"]);
        if (sel == "Stamina") {
            var itm = game.drawSpecificObject("OBJ_STRANGLER_DECREASE_STAMINA");
            game.context.character.addToInventory(itm);
        } else {
            var itm = game.drawSpecificObject("OBJ_STRANGLER_DECREASE_SANITY");
            game.context.character.addToInventory(itm);
        }

        /* TODO: Do on all characters!
        var list = game.context.allCharacters;
        for (var i = 0; i < list.length; ++i) {
        }
        */

        this.removeFromGame();
    },
    properties: {
        allyCount: { display: "Remaining Ally Cards" }
    }
}));
