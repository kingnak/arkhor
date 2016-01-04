
// TEST
var m = game.createMonster({
  tid: "MO_TEST",
  name: "TEST",
  description: "If you lose a Combat check against TEST, you are devoured",
  //dimension: Constants.Dimension.Circle,
  movementType: Constants.Movement.Stationary,
  awareness: -2,
  horrorAdjustment: -1,
  horrorDamage: 1,
  combatAdjustment: -2,
  combatDamage: 2,
  toughness: 1,
  onDamage: function(type, chr) {
      if (type == Constants.MonsterDamage.Combat) {
        chr.devour();
        return false;
      }
  }
  //attributes:
});
game.registerMonster(3, m);


/*
// BYAKHEE
var m = game.createMonster({
  id: "MO_Byakhee",
  name: "Byakhee",
  mythText:
    "It shrieked and flapped its wings. "+
    "I thought of the empty shell of a beetle, "+
    "and the soft loam of rotting earth. It cocked "+
    "its carapaced head at me, black teeth clicking "+
    "against each other.",
  //dimension: Constants.Dimension.Circle,
  movementType: Constants.Movement.Flying,
  awareness: -2,
  horrorAdjustment: -1,
  horrorDamage: 1,
  combatAdjustment: 0,
  combatDamage: 2,
  toughness: 1,
  //attributes: 
});
game.registerMonster(3, m);


// CHTHONIAN
var m = game.createMonster({
  id: "MO_CHTHONIAN",
  name: "Chthonian",
  description:
    "Instead of moving, roll a die. On a 4-6, "+
    "all investigators in Arkham lose 1 Stamina",
  //dimension: Constants.Dimension.Triangle,
  movementType: Constants.Movement.Special,
  awareness: +1,
  horrorAdjustment: -2,
  horrorDamage: 2,
  combatAdjustment: -3,
  combatDamage: 3,
  toughness: 3,
  //attributes:
  onMove: function() {
    var list = game.context.allCharacters;
    for (var i = 0; i < list.length; ++i) {
      var c = list[i];
      if (c.field.type == Constants.FieldType.Location || c.field.type == Constants.FieldType.Street) {
        c.damageStamina(1);
        c.commitDamage();
      }
    }
  }
});
game.registerMonster(3, m);

// CULTIST
var m = game.createMonster({
  id: "MO_Cultist",
  name: "Cultist",
  mythText:
    "la! la! Cthulhu Fthagen! "+
    "Ph'nglui mglw'nfah Cthulhu "+
    "R'lyeh ywah'nagl fhtagn!",
  //dimension: Constants.Dimension.Moon,
  movementType: Constants.Movement.Normal,
  awareness: -3,
  horrorAdjustment: 0,
  horrorDamage: 0,
  combatAdjustment: 1,
  combatDamage: 1,
  toughness: 1,
  //attributes: 
});
game.registerMonster(6, m);

// DARK YOUNG
var m = game.createMonster({
  id: "MO_DARK_YOUNG",
  name: "Dark Young",
  mythText:
    "The trees trembled, leaves fluttering "+
    "around us. The earth shook as the thuds "+
    "of giant hooved feet grew ever closer...",
  //dimension: Constants.Dimension.Hexagon,
  movementType: Constants.Movement.Stationary,
  awareness: -2,
  horrorAdjustment: 0,
  horrorDamage: 3,
  combatAdjustment: -1,
  combatDamage: 3,
  toughness: 3,
  attributes: [
    Constants.Monster.PhysicalResistance,
    Constants.Monster.Nightmarish_1
  ]
});
game.registerMonster(3, m);

// DHOLE
var m = game.createMonster({
  id: "MO_DHOLE",
  name: "Dhole",
  //dimension: Constants.Dimension.Circle,
  movementType: Constants.Movement.Normal,
  awareness: -1,
  horrorAdjustment: -1,
  horrorDamage: 4,
  combatAdjustment: -3,
  combatDamage: 4,
  toughness: 3,
  attributes: [
    Constants.Monster.PhysicalResistance,
    Constants.Monster.MagicalResistance,
    Constants.Monster.Nightmarish_1,
    Constants.Monster.Overwhelming_1
  ]
});
game.registerMonster(1, m);

// DIMENSIONAL SHAMBLER
var m = game.createMonster({
  id: "MO_DIMENSIONAL_SHAMBLER",
  name: "Dimensional Shambler",
  description:
    "If you fail a Combat Check against Dimensional "+
    "Shambler, you are lost in time and space.",
  //dimension: Constants.Dimension.Hexagon,
  movementType: Constants.Movement.Fast,
  awareness: -3,
  horrorAdjustment: -2,
  horrorDamage: 1,
  combatAdjustment: -2,
  combatDamage: 0,
  toughness: 1,
  onDamage: function(type, chr) {
    if (type == Constants.MonsterDamage.Combat) {
      chr.lostInSpaceAndTime();
      return false;
    }
  }
});
game.registerMonster(2, m);

// FIRE VAMPIRE
var m = game.createMonster({
  id: "MO_FIRE_VAMPIRE",
  name: "Fire Vampire",
  mythText:
    "A thousand tiny fires, the size of "+
    "match-flames, swept from the night sky. "+
    "The old man was suddenly radiant with light.",
  //dimension: Constants.Dimension.Star,
  movementType: Constants.Movement.Flying,
  awareness: 0,
  horrorAdjustment: 0,
  horrorDamage: 0,
  combatAdjustment: -2,
  combatDamage: 2,
  toughness: 1,
  attributes: [
    Constants.Monster.PhysicalImunity,
    Constants.Monster.Ambush
  ]
});
game.registerMonster(2, m);

// FLYING POLYP
var m = game.createMonster({
  id: "MO_FLYING_POLYP",
  name: "Flying Polyp",
  //dimension: Constants.Dimension.Hexagon,
  movementType: Constants.Movement.Flying,
  awareness: 0,
  horrorAdjustment: -2,
  horrorDamage: 4,
  combatAdjustment: -3,
  combatDamage: 3,
  toughness: 3,
  attributes: [
    Constants.Monster.PhysicalResistance,
    Constants.Monster.Nightmarish_1,
    Constants.Monster.Overwhelming_1
  ]
});
game.registerMonster(1, m);

// FORMLESS SPAWN
var m = game.createMonster({
  id: "MO_FORMLESS_SPAWN",
  name: "Formless Spawn",
  mythText: "From the darkened alley a "+
    "patch of darkness seemed to detach "+
    "itself, curling along the bricks and "+
    "rising to block our path.",
  //dimension: Constants.Dimension.Hexagon,
  movementType: Constants.Movement.Normal,
  awareness: 0,
  horrorAdjustment: -1,
  horrorDamage: 2,
  combatAdjustment: -2,
  combatDamage: 2,
  toughness: 2,
  attributes: [
    Constants.Monster.PhysicalImunity
  ]
});
game.registerMonster(2, m);

// GHOST
var m = game.createMonster({
  id: "MO_GHOST",
  name: "Ghost",
  //dimension: Constants.Dimension.Moon,
  movementType: Constants.Movement.Stationary,
  awareness: -3,
  horrorAdjustment: -2,
  horrorDamage: 2,
  combatAdjustment: -3,
  combatDamage: 2,
  toughness: 1,
  attributes: [
    Constants.Monster.PhysicalImunity,
    Constants.Monster.Undead
  ]
});
game.registerMonster(3, m);

// GHOUL
var m = game.createMonster({
  id: "MO_GHOUL",
  name: "Ghoul",
  mythText: "Most of the bodies, while roughly "+
    "bipedal, had a forward slumping, and a vaguely "+
    "canine cast. The texture of the majority was "+
    "a kind of unpleasant rubberiness.",
  //dimension: Constants.Dimension.Hexagon,
  movementType: Constants.Movement.Normal,
  awareness: -3,
  horrorAdjustment: 0,
  horrorDamage: 1,
  combatAdjustment: -1,
  combatDamage: 1,
  toughness: 1,
  attributes: [
    Constants.Monster.Ambush
  ]
});
game.registerMonster(3, m);

// GOD OF THE BLOODY TONGUE
var m = game.createMonster({
  id: "MO_GOD_OF_THE_BLOODY_TONGUE",
  name: "God of the Bloody Tongue",
  //dimension: Constants.Dimension.Triangle,
  movementType: Constants.Movement.Normal,
  awareness: +1,
  horrorAdjustment: -3,
  horrorDamage: 3,
  combatAdjustment: -4,
  combatDamage: 4,
  toughness: 4,
  attributes: [
    Constants.Monster.Mask,
    Constants.Monster.Endless,
    Constants.Monster.Overwhelming_1,
    Constants.Monster.Nightmarish_1
  ]
});
game.registerMonster(1, m);

// GUG
var m = game.createMonster({
  id: "MO_GUG",
  name: "Gug",
  mythText: "I caught confused glimpses "+
    "of a terrible, wrongly-angled mouth, "+
    "multi-jointed arms, and protruding eyes "+
    "as big as a man's head.",
  //dimension: Constants.Dimension.Slash,
  movementType: Constants.Movement.Normal,
  awareness: -2,
  horrorAdjustment: -1,
  horrorDamage: 2,
  combatAdjustment: -2,
  combatDamage: 4,
  toughness: 3,
  attributes: [
    Constants.Monster.Overwhelming_1
  ]
});
game.registerMonster(2, m);

// HIGH PRIEST
var m = game.createMonster({
  id: "MO_HIGH_PRIEST",
  name: "High Priest",
  mythText: "\"We will not cease our struggle,\" "+
    "intoned the robed figure, \"Until we have "+
    "built Carcosa upon this land of ruined Reason.\"",
  //dimension: Constants.Dimension.Plus,
  movementType: Constants.Movement.Normal,
  awareness: -2,
  horrorAdjustment: +1,
  horrorDamage: 1,
  combatAdjustment: -2,
  combatDamage: 2,
  toughness: 2,
  attributes: [
    Constants.Monster.MagicalImunity
  ]
});
game.registerMonster(1, m);

// MANIAC
var m = game.createMonster({
  id: "MO_MANIAC",
  name: "Maniac",
  description: "If the terror level is at least 6, "+
    "Maniac's fight rating increases to -2, his "+
    "combat damage increases to 3 Stamina, and "+
    "he gains Endless.",
  //dimension: Constants.Dimension.Moon,
  movementType: Constants.Movement.Normal,
  awareness: -1,
  horrorAdjustment: 0,
  horrorDamage: 0,
  combatAdjustment: +1,
  combatDamage: 1,
  toughness: 1,
  attributes: function() {
    if (game.context.terrorLevel >= 6) {
      return Constants.Monster.Endless;
    }
    return 0;
  },
  
  modifications: function() {
    if (game.context.terrorLevel >= 6) {
      return [
	{ property: Constants.Mods.Monster_CombatAdjustment, value: -3 },
	{ property: Constants.Mods.Monster_CombatDamage, value: 2 }
      ];
    }
    return null;
  }
});
game.registerMonster(3, m);

// MI-GO
var m = game.createMonster({
  id: "MO_MI_GO",
  name: "Mi-Go",
  description: "If you pass a Combat check "+
    "against Mi-Go, return it to the box and "+
    "draw 1 Unique Item.",
  //dimension: Constants.Dimension.Circle,
  movementType: Constants.Movement.Flying,
  awareness: -2,
  horrorAdjustment: -1,
  horrorDamage: 2,
  combatAdjustment: 0,
  combatDamage: 1,
  toughness: 1,
  onDefeat: function() {
    var obj = game.context.drawObject(Constants.ObjectType.UniqueItem);
    game.context.character.addToInventory(obj);
    return false;
  }
});
game.registerMonster(3, m);

// SHOGGOTH
var m = game.createMonster({
  id: "MO_SHOGGOTH",
  name: "Shoggoth",
  mythText: "The stench was unbearale, and "+
    "rising to the surface came a terrible cry. "+
    "\"Tekeli-li! Tekeli-li!\"",
  //dimension: Constants.Dimension.Diamond,
  movementType: Constants.Movement.Fast,
  awareness: -1,
  horrorAdjustment: -1,
  horrorDamage: 3,
  combatAdjustment: -1,
  combatDamage: 3,
  toughness: 3,
  attributes: [
    Constants.Monster.PhysicalResistance,
    Constants.Monster.Nightmarish_1
  ]
});
game.registerMonster(2, m);

// STAR SPAWN
var m = game.createMonster({
  id: "MO_STAR_SPAWN",
  name: "Star Spawn",
  mythText: "The bosun was the only one "+
    "left alive. We dragged him screaming "+
    "from the cargo hold. \"That thing!\" "+
    "he wept, \"Not a whale... not an island...\"",
  //dimension: Constants.Dimension.Plus,
  movementType: Constants.Movement.Normal,
  awareness: -1,
  horrorAdjustment: -3,
  horrorDamage: 2,
  combatAdjustment: -3,
  combatDamage: 3,
  toughness: 3,
});
game.registerMonster(2, m);

// VAMPIRE
var m = game.createMonster({
  id: "MO_VAMPIRE",
  name: "Vampire",
  mythText: "It was a beast born in grave. "+
    "It stretched its hands toward us and "+
    "curled its lips, drawn to the blood "+
    "in our veins.",
  //dimension: Constants.Dimension.Moon,
  movementType: Constants.Movement.Normal,
  awareness: -3,
  horrorAdjustment: 0,
  horrorDamage: 2,
  combatAdjustment: -3,
  combatDamage: 3,
  toughness: 2,
  attributes: [
    Constants.Monster.Undead,
    Constants.Monster.PhysicalResistance
  ]
});
game.registerMonster(1, m);

// WARLOCK
var m = game.createMonster({
  id: "MO_WARLOCK",
  name: "Warlock",
  description: "If you pass a Combat check "+
    "against Warlock, return it to the box "+
    "and gain 2 Clue tokens",
  //dimension: Constants.Dimension.Circle,
  movementType: Constants.Movement.Stationary,
  awareness: -2,
  horrorAdjustment: -1,
  horrorDamage: 1,
  combatAdjustment: -3,
  combatDamage: 1,
  toughness: 2,
  attributes: [
    Constants.Monster.MagicalImunity
  ],
  onDefeat: function(chr) {
    chr.addClue(2);
    return false;
  }
});
game.registerMonster(2, m);

// WHTCH
var m = game.createMonster({
  id: "MO_WITCH",
  name: "Witch",
  mythText: "She was lovely and terrible all "+
    "at once. A strange light shone in her "+
    "eyes as she changed, \"Iä! Iä! Shub-Niggurath! "+
    "The Goat with a Thousand Young!\"",
  //dimension: Constants.Dimension.Circle,
  movementType: Constants.Movement.Normal,
  awareness: -1,
  horrorAdjustment: 0,
  horrorDamage: 0,
  combatAdjustment: -3,
  combatDamage: 2,
  toughness: 1,
  attributes: [
    Constants.Monster.MagicalResistance
  ]
});
game.registerMonster(2, m);

// ZOMBIE
var m = game.createMonster({
  id: "MO_ZOMBIE",
  name: "Zombie",
  mythText: "She screamed and fired again, "+
    "but still the thing shambled on, teeth "+
    "dripping as it groaned its horrible cry.",
  //dimension: Constants.Dimension.Moon,
  movementType: Constants.Movement.Normal,
  awareness: +1,
  horrorAdjustment: -1,
  horrorDamage: 1,
  combatAdjustment: -1,
  combatDamage: 2,
  toughness: 1,
  attributes: [
    Constants.Monster.Undead
  ]
});
game.registerMonster(3, m);
*/
