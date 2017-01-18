game.utility = {
    curseCharacter: function(chr, force) {
        if (force == undefined) force = false;
        force = !!force;

        var obj = game.drawSpecificObject(BlessingCurse.Curse);
        chr.addToInventory(obj);
        if (force) {
            obj = game.drawSpecificObject(BlessingCurse.Curse);
            chr.addToInventory(obj);
        }
    },

    blessCharacter: function(chr, force) {
        if (force == undefined) force = false;
        force = !!force;

        var obj = game.drawSpecificObject(BlessingCurse.Blessing);
        chr.addToInventory(obj);
        if (force) {
            obj = game.drawSpecificObject(BlessingCurse.Blessing);
            chr.addToInventory(obj);
        }
    }
}
