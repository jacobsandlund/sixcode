'use strict';
global.ApiSet = {};
(function () {

var newPointers = new Uint32Array(4);
var tempHash = new Uint8Array(20);

global.set = function (pointer) {
    var pointer32 = pointer >> 2;
    var moldIndex = $table.data32[pointer32 + Table.data32_moldIndex];
    var mold8 = moldIndex * Mold.data8_size;
    var numChildren = $mold.data8[mold8 + Mold.data8_numChildren];

    // Set newPointers
    var i;
    for (i = 0; i < numChildren; i++) {
        newPointers[i] = $table.data32[pointer32 + i];
    }

    for (i = 1; i < arguments.length; i += 2) {
        var childIndex = arguments[i];
        if (typeof childIndex === 'undefined') {
            throw new Error('Trying to set undefined child');
        } else if (childIndex >= numChildren) {
            throw new Error('Trying to set child ' + childIndex + ' out of ' + numChildren);
        }
        newPointers[childIndex] = arguments[i + 1];
    }

    return ApiSet._create(moldIndex, newPointers);
};

ApiSet._create = function (moldIndex, newPointers) {
    var mold8 = moldIndex * Mold.data8_size;
    var numChildren = $mold.data8[mold8 + Mold.data8_numChildren];

    // Write to mold
    var mold32 = moldIndex * Mold.data32_size;
    var fileStart = $mold.data32[mold32 + Mold.data32_fileStart];
    var fileEnd = $mold.data32[mold32 + Mold.data32_fileEnd];
    Mold.fillHoles($mold, moldIndex, newPointers, 0);

    // Hash and store in table
    Sha1.hash($mold.fileArray, fileStart, fileEnd, tempHash, 0);
    var pointer = Table.findPointer($table, tempHash, 0);
    if (pointer > 0) {
        return pointer;
    }

    pointer = ~pointer;
    Table.setHash($table, pointer, tempHash, 0);
    var pointer32 = pointer >> 2;
    $table.data8[Table.typeOffset(pointer)] = Type.tree;
    $table.data32[pointer32 + Table.data32_moldIndex] = moldIndex;
    var i;
    for (i = 0; i < numChildren; i++) {
        $table.data32[pointer32 + i] = newPointers[i];
    }

    return pointer;
};

})();
