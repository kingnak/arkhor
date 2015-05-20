
var derringer = game.createObject({
    id: "OB_.18_Derringer",
    name: ".18 Derringer",
    type: Constants.ObjectType.CommonItem,
    description: ".18 Derringer cannot be lost or stolen unless you choose to allow it.",
    hands: 1,
    price: 3,
    modifications: { property: Constants.Mods.Skill_Combat, value: 2}
});

game.registerMultiObject(2, derringer);
