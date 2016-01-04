

// ELDER THING
var m = game.createMonster({
  tid: "MO_ELDER_THING",
  name: "Elder Thing",
  description:
    "When you fail a Combat Check against "+
    "Elder Thing, you must discard 1 of your "+
    "Weapons or Spells (your choice), if able.",
  //dimension: Constants.Dimension.Diamond,
  movementType: Constants.Movement.Normal,
  awareness: -2,
  horrorAdjustment: -3,
  horrorDamage: 2,
  combatAdjustment: 0,
  combatDamage: 1,
  toughness: 2,
  
});
game.registerMonster(2, m);

// HAUNTER OF THE DARK
var m = game.createMonster({
  tid: "MO_HAUNTER_OF_THE_DARK",
  name: "Haunter of the Dark",
  description: "If the Blackest Night card is "+
    "in play, Haunter of the Dark's fighting "+
    "rating increases to -5.",
  //dimension: Constants.Dimension.Square,
  movementType: Constants.Movement.Flying,
  awareness: -3,
  horrorAdjustment: -2,
  horrorDamage: 2,
  combatAdjustment: -2,
  combatDamage: 2,
  toughness: 2,
  attributes: [
    Constants.Monster.Mask,
    Constants.Monster.Endless
  ]
});
game.registerMonster(1, m);

// HOUND OF TINDALOS
var m = game.createMonster({
  tid: "MO_HOUND_OF_TINDALOS",
  name: "Hound of Tindalos",
  description: "When it moves, the Hound of "+
    "Tindalos moves directly to the nearest "+
    "investigator inside a location in Arkham "+
    "Other than the Hospital or Asylum).",
  //dimension: Constants.Dimension.Square,
  movementType: Constants.Movement.Special,
  awareness: -1,
  horrorAdjustment: -2,
  horrorDamage: 4,
  combatAdjustment: -1,
  combatDamage: 3,
  toughness: 2,
  attributes: [
    Constants.Monster.PhysicalImunity
  ],
  onMove: function() {
    // TODO
  }
});
game.registerMonster(2, m);


// NIGHTGAUNT
var m = game.createMonster({
  tid: "MO_NIGHTGAUNT",
  name: "Nightgaunt",
  description: "When you fail a Combat "+
    "check against Nightgaunt, you are drawn "+
    "through the nearest open gate. If two or "+
    "more gates are the same distance from you, "+
    "you choose which gate you are drawn through.",
    /* NOTE: If in Other World, nearest gate is 
     * back to Arkham.
     * NOTE: If no gate open, fight ends with no effect.
     * NOTE: Also drawn through when failing evade check.
     */ 
  //dimension: Constants.Dimension.Slash,
  movementType: Constants.Movement.Flying,
  awareness: -2,
  horrorAdjustment: -1,
  horrorDamage: 1,
  combatAdjustment: -2,
  combatDamage: 0,
  toughness: 2,
});
game.registerMonster(2, m);

// THE BLACK MAN
var m = game.createMonster({
  tid: "MO_THE_BLACK_MAN",
  name: "The Black Man",
  description: "Before making a Horror Check, "+
    "pass a Luck (-1) check or be devoured. "+
    "If you pass, gain 2 Clue tokens. In either "+
    "case, return the black Man to the cup.",
  //dimension: Constants.Dimension.Moon,
  movementType: Constants.Movement.Normal,
  awareness: -3,
  horrorAdjustment: 0,
  horrorDamage: 0,
  combatAdjustment: 0,
  combatDamage: 0,
  toughness: 1,
  attributes: [
    Constants.Monster.Mask,
    Constants.Monster.Endless
  ]
});
game.registerMonster(1, m);

// THE BLOATED WOMAN
var m = game.createMonster({
  tid: "MO_THE_BLOATED_WOMAN",
  name: "The Bloated Woman",
  description: "Before making a Horror "+
    "check, pass a Will (-2) check or "+
    "automatically fail the Horror check "+
    "and the Combat check.",
  //dimension: Constants.Dimension.Hexagon,
  movementType: Constants.Movement.Normal,
  awareness: -1,
  horrorAdjustment: -1,
  horrorDamage: 2,
  combatAdjustment: -2,
  combatDamage: 2,
  toughness: 2,
  attributes: [
    Constants.Monster.Mask,
    Constants.Monster.Endless
  ]
});
game.registerMonster(1, m);

// THE DARK PHAROAH
var m = game.createMonster({
  tid: "MO_THE_DARK_PHAROAH",
  name: "The Dark Pharoah",
  description: "Use Lore in combat with the "+
    "Dark Pharoah instead of Fight.",
  //dimension: Constants.Dimension.Slash,
  movementType: Constants.Movement.Normal,
  awareness: -1,
  horrorAdjustment: -1,
  horrorDamage: 1,
  combatAdjustment: -3,
  combatDamage: 3,
  toughness: 2,
  attributes: [
    Constants.Monster.Mask,
    Constants.Monster.Endless
  ]
});
game.registerMonster(1, m);


