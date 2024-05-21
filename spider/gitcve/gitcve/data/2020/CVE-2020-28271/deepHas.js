var indexFalse,
    indexTrue,
    indexer,
    reduce,
    add,
    has,
    get,
    set;

function indexer(set) {
    return function(obj, i) {
        "use strict";
        try {
            if (obj && i && obj.hasOwnProperty(i)) {
                return obj[i];
            } else if (obj && i && set) {
                obj[i] = {};
                return obj[i];
            }
            return;
        } catch(ex) {
            console.error(ex);
            return;
        }
    };
}

indexTrue = indexer(true);
indexFalse = indexer(false);

function reduce(obj, str) {
    "use strict";
    try {
        if ( typeof str !== "string") {
            return;
        }
        if ( typeof obj !== "object") {
            return;
        }
        return str.split('.').reduce(indexFalse, obj);

    } catch(ex) {
        console.error(ex);
        return;
    }

}

function add(obj, str, val) {
    "use strict";
    try {
        if ( typeof str !== "string") {
            return;
        }
        if ( typeof obj !== "object") {
            return;
        }
        if (!val) {
            return;
        }
        var items = str.split('.');
        var initial = items.slice(0, items.length - 1);
        var last = items.slice(items.length - 1);
        var test = initial.reduce(indexTrue, obj);
        test[last] = val;
    } catch(ex) {
        console.error(ex);
        return;
    }
}

function has(target, path) {
    "use strict";
    try {
        var test = reduce(target, path);
        if ( typeof test !== "undefined") {
            return true;
        }
        return false;
    } catch(ex) {
        console.error(ex);
        return;
    }
}

function get(target, path) {
    "use strict";
    try {
        return reduce(target, path);
    } catch(ex) {
        console.error(ex);
        return;
    }
}

function set(target, path, val) {
    "use strict";
    try {
        return add(target, path, val);
    } catch(ex) {
        console.error(ex);
        return;
    }
}

exports.has = has;
exports.get = get;
exports.set = set;
