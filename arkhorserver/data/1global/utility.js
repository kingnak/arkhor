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

game.utility.drawRandomItem = function(type, message, id) {
    return game.utility.selectRandomItem(type, message, id, 1);
}

game.utility.selectRandomItem = function(type, message, id, count) {
    var objs = game.context.drawMultipleObjects(type, message, id, count, 1, 1);
    if (objs.length > 0) {
        CHAR.addToInventory(objs[0]);
        return objs[0];
    }
    return null;
}

game.utility.randomField = function(type) {
    var fields = game.context.allFields(type);
    var idx = Math.floor(Math.random() * fields.length);
    return fields[idx];
}
