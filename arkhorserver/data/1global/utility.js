game.utility.blessCharacter = function(chr, force) {
    if (force == undefined) force = false;
    force = !!force;

    if (force && game.utility.isCharacterCursed(chr)) {
        var obj = game.drawSpecificObject(BlessingCurse.Blessing);
        chr.addToInventory(obj);
    }
    var obj = game.drawSpecificObject(BlessingCurse.Blessing);
    chr.addToInventory(obj);
};

game.utility.curseCharacter = function(chr, force) {
    if (force == undefined) force = false;
    force = !!force;

    if (force && game.utility.isCharacterBlessed(chr)) {
        var obj = game.drawSpecificObject(BlessingCurse.Curse);
        chr.addToInventory(obj);
    }
    var obj = game.drawSpecificObject(BlessingCurse.Curse);
    chr.addToInventory(obj);
};

game.utility.isCharacterBlessed = function(chr) {
    if (chr.hasObject(BlessingCurse.Blessing)) {
        return true;
    }
    return false;
};

game.utility.isCharacterCursed = function(chr) {
    if (chr.hasObject(BlessingCurse.Curse)) {
        return true;
    }
    return false;
};
