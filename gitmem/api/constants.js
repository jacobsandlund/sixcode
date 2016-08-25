'use strict';
global.Constants = {};
(function () {

Constants.initialize = function (minNumber, maxPositive) {
    Constants.emptyString = $.nextIndex++;
    $[Constants.emptyString] = hash('');

    var emptyTree = createZero({
        '.empty': $[Constants.emptyString],
    });
    var pointer32 = emptyTree >> 2;
    var moldIndex = $table.data32[pointer32 + Table.data32_moldIndex];
    $mold.data8[moldIndex * Mold.data8_size + Mold.data8_numChildren] = 0;
    Constants.emptyTree = $.nextIndex++;
    $[Constants.emptyTree] = emptyTree;

    var zeroHashArray = new Uint8Array(20);
    var zeroHash = ~Table.findPointer($table, zeroHashArray, 0);
    Table.setHash($table, zeroHash, zeroHashArray, 0);
    $table.data8[Table.typeOffset(zeroHash)] = Type.pending;
    Constants.zeroHash = $.nextIndex++;
    $[Constants.zeroHash] = zeroHash;

    var maxNegative = -minNumber;
    Constants.$positive = new Uint32Array(maxPositive + 1);
    Constants.$negative = new Uint32Array(maxNegative + 1);
    var i;
    for (i = 0; i <= maxPositive; i++) {
        Constants.$positive[i] = hash(i);
    }
    for (i = 0; i <= maxNegative; i++) {
        Constants.$negative[i] = hash(-i);
    }
};

Constants.integer = function (integer) {
    if (integer >= 0) {
        return Constants.$positive[integer];
    } else {
        return Constants.$negative[-integer];
    }
};

})();
