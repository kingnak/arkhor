
var derringer = game.createObject({
    tid: "OB_.18_Derringer",
    name: ".18 Derringer",
    type: Constants.ObjectType.CommonItem,
    description: ".18 Derringer cannot be lost or stolen unless you choose to allow it.",
    hands: 1,
    price: 3,
    modifications: { property: Constants.Mods.Damage_Physical, value: 2}
});

game.registerMultiObject(2, derringer);
