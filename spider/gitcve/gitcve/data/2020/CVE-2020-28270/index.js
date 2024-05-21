(function (global, factory) {
	typeof exports === 'object' && typeof module !== 'undefined' ? factory(exports) :
	typeof define === 'function' && define.amd ? define(['exports'], factory) :
	(global = typeof globalThis !== 'undefined' ? globalThis : global || self, factory(global.ObjectHierarchyAccess = {}));
}(this, (function (exports) { 'use strict';

	function normalizeDescriptor(info) {
	    if (typeof info === 'object' && info !== null) {
	        return info;
	    }
	    else if (typeof info === 'function') {
	        return {
	            getName: info,
	            value: {}
	        };
	    }
	    else {
	        return {
	            name: info,
	            value: {}
	        };
	    }
	}

	function isArray(source) {
	    return Array.isArray(source) || source instanceof Array;
	}
	function isObject(source) {
	    return typeof source === 'object' && source !== null;
	}
	function getOwnEnumerablePropKeys(target) {
	    var keys = Object.keys(target);
	    if (Object.getOwnPropertySymbols) {
	        var symbols = Object.getOwnPropertySymbols(target).filter(function (symbol) {
	            var descriptor = Object.getOwnPropertyDescriptor(target, symbol);
	            return descriptor && descriptor.enumerable;
	        });
	        if (symbols.length) {
	            keys.push.apply(keys, symbols);
	        }
	    }
	    return keys;
	}
	function cloneContainer(from) {
	    if (isArray(from)) {
	        return [];
	    }
	    else if (isObject(from)) {
	        return {};
	    }
	    else {
	        return from;
	    }
	}
	function getPropName(current, descriptor) {
	    var name = descriptor.name, getName = descriptor.getName;
	    if (name !== undefined) {
	        return name;
	    }
	    if (getName) {
	        return getName.call(current, current);
	    }
	}
	function getNonEmptyPropName(current, descriptor) {
	    var name = getPropName(current, descriptor);
	    return name !== undefined ? name : 'undefined';
	}
	function getPropNames(current, descriptor) {
	    var names = descriptor.names, getNames = descriptor.getNames;
	    if (names !== undefined) {
	        return isArray(names) ? names : [names];
	    }
	    if (getNames) {
	        var gotNames = getNames.call(current, current);
	        if (gotNames !== undefined) {
	            return isArray(gotNames) ? gotNames : [gotNames];
	        }
	    }
	    return getOwnEnumerablePropKeys(current);
	}

	function generate(target, hierarchies, forceOverride) {
	    var current = target;
	    hierarchies.forEach(function (info) {
	        var descriptor = normalizeDescriptor(info);
	        var value = descriptor.value, type = descriptor.type, create = descriptor.create, override = descriptor.override, created = descriptor.created, skipped = descriptor.skipped, got = descriptor.got;
	        var name = getNonEmptyPropName(current, descriptor);
	        if (forceOverride || override || !current[name] || typeof current[name] !== 'object') {
	            var obj = value ? value :
	                type ? new type() :
	                    create ? create.call(current, current, name) :
	                        {};
	            current[name] = obj;
	            if (created) {
	                created.call(current, current, name, obj);
	            }
	        }
	        else {
	            if (skipped) {
	                skipped.call(current, current, name, current[name]);
	            }
	        }
	        var parent = current;
	        current = current[name];
	        if (got) {
	            got.call(parent, parent, name, current);
	        }
	    });
	    return current;
	}
	function setupIfUndef(target, hierarchies) {
	    return generate(target, hierarchies);
	}
	function setup(target, hierarchies) {
	    var current = generate(target, hierarchies.slice(0, -1));
	    var last = generate(current, hierarchies.slice(-1), true);
	    return { current: current, last: last };
	}

	function _parseArgs(others) {
	    var value = others[others.length - 1];
	    var rest = Array.prototype.concat.apply([], others.slice(0, -1)); // exclude `value`
	    var hierarchies = rest.slice(0, -1);
	    var prop = rest[rest.length - 1];
	    return { hierarchies: hierarchies, prop: prop, value: value };
	}
	function set(optionalTarget) {
	    var others = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        others[_i - 1] = arguments[_i];
	    }
	    var _a = _parseArgs(others), hierarchies = _a.hierarchies, prop = _a.prop, value = _a.value;
	    var target = optionalTarget || {};
	    var current = setupIfUndef(target, hierarchies);
	    current[prop] = value;
	    return target;
	}
	function assign(target) {
	    var others = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        others[_i - 1] = arguments[_i];
	    }
	    var _a = _parseArgs(others), hierarchies = _a.hierarchies, prop = _a.prop, value = _a.value;
	    var current = setupIfUndef(target, hierarchies);
	    current[prop] = value;
	    return current;
	}
	function put(target) {
	    var others = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        others[_i - 1] = arguments[_i];
	    }
	    var _a = _parseArgs(others), hierarchies = _a.hierarchies, prop = _a.prop, value = _a.value;
	    var current = setupIfUndef(target, hierarchies);
	    current[prop] = value;
	    return value;
	}
	function setIfUndef(optionalTarget) {
	    var others = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        others[_i - 1] = arguments[_i];
	    }
	    var _a = _parseArgs(others), hierarchies = _a.hierarchies, prop = _a.prop, value = _a.value;
	    var target = optionalTarget || {};
	    var current = setupIfUndef(target, hierarchies);
	    if (current[prop] === undefined) {
	        current[prop] = value;
	    }
	    return target;
	}
	function assignIfUndef(target) {
	    var others = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        others[_i - 1] = arguments[_i];
	    }
	    var _a = _parseArgs(others), hierarchies = _a.hierarchies, prop = _a.prop, value = _a.value;
	    var current = setupIfUndef(target, hierarchies);
	    if (current[prop] === undefined) {
	        current[prop] = value;
	    }
	    return current;
	}
	function putIfUndef(target) {
	    var others = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        others[_i - 1] = arguments[_i];
	    }
	    var _a = _parseArgs(others), hierarchies = _a.hierarchies, prop = _a.prop, value = _a.value;
	    var current = setupIfUndef(target, hierarchies);
	    if (current[prop] === undefined) {
	        current[prop] = value;
	    }
	    return current[prop];
	}

	function _normalizeHierarchies(hierarchies) {
	    var result = Array.prototype.concat.apply([], hierarchies);
	    return result;
	}
	function setProp(optionalTarget) {
	    var hierarchies = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        hierarchies[_i - 1] = arguments[_i];
	    }
	    var arrHierarchies = _normalizeHierarchies(hierarchies);
	    var target = optionalTarget || {};
	    setup(target, arrHierarchies);
	    return target;
	}
	function assignProp(target) {
	    var hierarchies = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        hierarchies[_i - 1] = arguments[_i];
	    }
	    var arrHierarchies = _normalizeHierarchies(hierarchies);
	    var current = setup(target, arrHierarchies).current;
	    return current;
	}
	function putProp(target) {
	    var hierarchies = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        hierarchies[_i - 1] = arguments[_i];
	    }
	    var arrHierarchies = _normalizeHierarchies(hierarchies);
	    var last = setup(target, arrHierarchies).last;
	    return last;
	}
	function setPropIfUndef(optionalTarget) {
	    var hierarchies = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        hierarchies[_i - 1] = arguments[_i];
	    }
	    var arrHierarchies = _normalizeHierarchies(hierarchies);
	    var target = optionalTarget || {};
	    setupIfUndef(target, arrHierarchies);
	    return target;
	}
	function assignPropIfUndef(target) {
	    var hierarchies = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        hierarchies[_i - 1] = arguments[_i];
	    }
	    var arrHierarchies = _normalizeHierarchies(hierarchies);
	    var current = setupIfUndef(target, arrHierarchies.slice(0, -1));
	    setupIfUndef(current, arrHierarchies.slice(-1));
	    return current;
	}
	function putPropIfUndef(target) {
	    var hierarchies = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        hierarchies[_i - 1] = arguments[_i];
	    }
	    var arrHierarchies = _normalizeHierarchies(hierarchies);
	    return setupIfUndef(target, arrHierarchies);
	}

	function normalizeDescriptor$1(info) {
	    if (typeof info === 'object') {
	        return info;
	    }
	    else if (typeof info === 'function') {
	        return {
	            getValue: info
	        };
	    }
	    else {
	        return {
	            name: info
	        };
	    }
	}
	function getNameValue(current, descriptor) {
	    var getValue = descriptor.getValue;
	    var name = getPropName(current, descriptor);
	    var value;
	    if (name !== undefined) {
	        value = current[name];
	    }
	    else {
	        name = 'undefined';
	        if (getValue) {
	            value = getValue.call(current, current);
	        }
	    }
	    var got = descriptor.got;
	    if (got) {
	        got.call(current, current, name, value);
	    }
	    return { name: name, value: value };
	}

	function get(target) {
	    var rest = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        rest[_i - 1] = arguments[_i];
	    }
	    var hierarchies = Array.prototype.concat.apply([], rest);
	    var current = target;
	    if (current !== undefined && current !== null) {
	        hierarchies.every(function (info) {
	            var descriptor = normalizeDescriptor$1(info);
	            var value = getNameValue(current, descriptor).value;
	            current = value;
	            return current;
	        });
	    }
	    return current;
	}

	function exist(target) {
	    var rest = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        rest[_i - 1] = arguments[_i];
	    }
	    if (target === undefined || target === null) {
	        return false;
	    }
	    var hierarchies = Array.prototype.concat.apply([], rest);
	    var current = target;
	    for (var i = 0; i < hierarchies.length; i++) {
	        var prop = hierarchies[i];
	        if (!current || !(prop in current)) {
	            return false;
	        }
	        current = current[prop];
	    }
	    return true;
	}

	function _parseArgs$1(others) {
	    var callback = others[others.length - 1];
	    var hierarchies = Array.prototype.concat.apply([], others.slice(0, -1)); // exclude `callback`
	    return { hierarchies: hierarchies, callback: callback };
	}
	function traverse(target) {
	    var others = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        others[_i - 1] = arguments[_i];
	    }
	    var _a = _parseArgs$1(others), hierarchies = _a.hierarchies, callback = _a.callback;
	    var current = target;
	    if (current !== undefined && current !== null) {
	        hierarchies.every(function (info) {
	            var descriptor = normalizeDescriptor$1(info);
	            var _a = getNameValue(current, descriptor), name = _a.name, value = _a.value;
	            var parent = current;
	            current = value;
	            var result = callback.call(parent, parent, name, current);
	            return result !== false;
	        });
	    }
	}
	function traverseReverse(target) {
	    var others = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        others[_i - 1] = arguments[_i];
	    }
	    var _a = _parseArgs$1(others), hierarchies = _a.hierarchies, callback = _a.callback;
	    var current = target;
	    if (current !== undefined && current !== null) {
	        var params_1 = [];
	        hierarchies.every(function (info) {
	            var descriptor = normalizeDescriptor$1(info);
	            var _a = getNameValue(current, descriptor), name = _a.name, value = _a.value;
	            var parent = current;
	            current = value;
	            params_1.push({ parent: parent, name: name, current: current });
	            return current;
	        });
	        for (var i = params_1.length - 1; i >= 0; i--) {
	            var item = params_1[i];
	            var result = callback.call(item.parent, item.parent, item.name, item.current);
	            if (result === false) {
	                break;
	            }
	        }
	    }
	}

	function array2map(arr, key, value) {
	    if (!isArray(arr)) {
	        return;
	    }
	    var result = {};
	    for (var i = 0; i < arr.length; i++) {
	        var item = arr[i];
	        var keyProp = get(item, key);
	        var valueProp = get(item, value);
	        result[keyProp] = valueProp;
	    }
	    return result;
	}

	function map2array(obj, keyName, valueName) {
	    if (!obj) {
	        return;
	    }
	    var result = [];
	    getOwnEnumerablePropKeys(obj).forEach(function (key) {
	        var _a;
	        var value = obj[key];
	        var keyProp = typeof keyName === 'function' ? keyName.call(obj, obj, key, value) : keyName;
	        var valueProp = typeof valueName === 'function' ? valueName.call(obj, obj, key, value) : valueName;
	        result.push((_a = {},
	            _a[keyProp] = key,
	            _a[valueProp] = value,
	            _a));
	    });
	    return result;
	}

	function normalizeDescriptor$2(info) {
	    if (isArray(info)) {
	        return {
	            names: info
	        };
	    }
	    else if (typeof info === 'object' && info !== null) {
	        return info;
	    }
	    else if (typeof info === 'function') {
	        return {
	            getNames: info
	        };
	    }
	    else {
	        return {
	            names: info
	        };
	    }
	}
	function getMappedNameValue(current, name, descriptor) {
	    var got = descriptor.got, mapName = descriptor.mapName, mapValue = descriptor.mapValue, mapped = descriptor.mapped;
	    var next = current[name];
	    if (got) {
	        got.call(current, current, name, next);
	    }
	    var mappedName = mapName ? mapName.call(current, current, name, next) : name;
	    var mappedValue = mapValue ? mapValue.call(current, current, name, next) : next;
	    if (mapped) {
	        mapped.call(current, current, mappedName, mappedValue);
	    }
	    return { mappedName: mappedName, mappedValue: mappedValue };
	}

	function generate$1(current, result, hierarchies, index) {
	    var descriptor = normalizeDescriptor$2(hierarchies[index]);
	    var names = getPropNames(current, descriptor);
	    var lastIndex = hierarchies.length - 1;
	    names.forEach(function (name) {
	        if (name in current) {
	            var _a = getMappedNameValue(current, name, descriptor), mappedName = _a.mappedName, mappedValue = _a.mappedValue;
	            if (index < lastIndex) {
	                result[mappedName] = cloneContainer(mappedValue);
	            }
	            else {
	                result[mappedName] = mappedValue;
	            }
	            if (index < lastIndex && typeof mappedValue === 'object' && mappedValue !== null) {
	                generate$1(mappedValue, result[mappedName], hierarchies, index + 1);
	            }
	        }
	    });
	}
	function select(target) {
	    var hierarchyProps = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        hierarchyProps[_i - 1] = arguments[_i];
	    }
	    var result;
	    var current = target;
	    if (current !== undefined && current !== null) {
	        result = cloneContainer(current);
	        generate$1(current, result, hierarchyProps, 0);
	    }
	    return result;
	}

	function find(current, result, hierarchies, index) {
	    var descriptor = normalizeDescriptor$2(hierarchies[index]);
	    var names = getPropNames(current, descriptor);
	    var lastIndex = hierarchies.length - 1;
	    names.forEach(function (name) {
	        if (name in current) {
	            var mappedValue = getMappedNameValue(current, name, descriptor).mappedValue;
	            if (index < lastIndex) {
	                find(mappedValue, result, hierarchies, index + 1);
	            }
	            else {
	                result.push(mappedValue);
	            }
	        }
	    });
	}
	function pick(target) {
	    var hierarchyProps = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        hierarchyProps[_i - 1] = arguments[_i];
	    }
	    var result = [];
	    var current = target;
	    if (current !== undefined && current !== null) {
	        find(current, result, hierarchyProps, 0);
	    }
	    return result;
	}

	function normalizeDescriptor$3(info) {
	    if (typeof info === 'object' && info !== null) {
	        return info;
	    }
	    else if (typeof info === 'function') {
	        return {
	            by: info
	        };
	    }
	    else {
	        return {};
	    }
	}

	function _createContainer(descriptor, target) {
	    var type = descriptor.type, create = descriptor.create;
	    if (type) {
	        return new type();
	    }
	    else if (create) {
	        return create.call(target, target);
	    }
	    else {
	        return {};
	    }
	}
	function group(target) {
	    var params = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        params[_i - 1] = arguments[_i];
	    }
	    if (!params.length) {
	        return target;
	    }
	    var descriptors = Array.prototype.concat.apply([], params).map(normalizeDescriptor$3).filter(function (d) { return d.by; });
	    if (!descriptors.length) {
	        return target;
	    }
	    var lastIndex = descriptors.length - 1;
	    var keys = getOwnEnumerablePropKeys(target);
	    var rootContainer;
	    keys.forEach(function (key) {
	        var child = target[key];
	        var prevContainer;
	        var prevName;
	        descriptors.forEach(function (descriptor, index) {
	            var by = descriptor.by;
	            if (index === 0) {
	                if (!rootContainer) {
	                    rootContainer = _createContainer(descriptor, target);
	                }
	                prevContainer = rootContainer;
	            }
	            else {
	                if (!prevContainer[prevName]) {
	                    prevContainer[prevName] = _createContainer(descriptor, target);
	                }
	                prevContainer = prevContainer[prevName];
	            }
	            var groupName = by.call(target, target, key, child);
	            if (index !== lastIndex) {
	                prevName = groupName;
	            }
	            else {
	                if (!prevContainer[groupName]) {
	                    prevContainer[groupName] = cloneContainer(target);
	                }
	                var currentContainer = prevContainer[groupName];
	                if (isArray(currentContainer)) {
	                    currentContainer.push(child);
	                }
	                else {
	                    currentContainer[key] = child;
	                }
	            }
	        });
	    });
	    return rootContainer;
	}

	function _getDimTypes(input, maxDim) {
	    if (maxDim === void 0) { maxDim = 16; }
	    var types = [];
	    if (isObject(input)) {
	        var type = isArray(input) ? Array : Object;
	        var dimItems = [input];
	        var _loop_1 = function (iDim) {
	            var nextType = Array;
	            var nextDimItems = [];
	            dimItems.forEach(function (dimItem) {
	                getOwnEnumerablePropKeys(dimItem).forEach(function (key) {
	                    var nextDimItem = dimItem[key];
	                    if (isObject(nextDimItem)) {
	                        if (!isArray(nextDimItem)) {
	                            nextType = Object;
	                        }
	                        nextDimItems.push(nextDimItem);
	                    }
	                });
	            });
	            types.push(type);
	            if (!nextDimItems.length) {
	                return "break";
	            }
	            type = nextType;
	            dimItems = nextDimItems;
	        };
	        for (var iDim = 0; iDim <= maxDim; iDim++) {
	            var state_1 = _loop_1(iDim);
	            if (state_1 === "break")
	                break;
	        }
	    }
	    return types;
	}
	function redim(data) {
	    var newDimsOrder = [];
	    for (var _i = 1; _i < arguments.length; _i++) {
	        newDimsOrder[_i - 1] = arguments[_i];
	    }
	    if (!data) {
	        return data;
	    }
	    // newDims: new order of old dims
	    var newDims = Array.prototype.concat.apply([], newDimsOrder);
	    if (!newDims.length) {
	        return data;
	    }
	    var oldDimMin = Math.min.apply(Math, newDims);
	    if (oldDimMin < 0) {
	        return;
	    }
	    var oldDimMax = Math.max.apply(Math, newDims);
	    var newDimMax = newDims.length - 1;
	    var dimTypes = _getDimTypes(data, oldDimMax);
	    if (!dimTypes.length || oldDimMax >= dimTypes.length) {
	        return;
	    }
	    var result = new dimTypes[newDims[0]];
	    var _walk = function _walk(path, current, currentDim) {
	        if (currentDim <= oldDimMax) {
	            getOwnEnumerablePropKeys(current).forEach(function (key) {
	                var nextDim = currentDim + 1;
	                if (exist(current, key)) {
	                    _walk(path.concat(key), current[key], nextDim);
	                }
	            });
	        }
	        else {
	            var newHierarchyDescriptors = newDims.map((function (oldDim, newDim) {
	                return newDim < newDimMax ? {
	                    name: path[oldDim],
	                    type: dimTypes[newDims[newDim + 1]],
	                } : {
	                    name: path[oldDim],
	                    value: current
	                };
	            }));
	            setProp(result, newHierarchyDescriptors);
	        }
	    };
	    _walk([], data, 0);
	    return result;
	}

	exports.array2map = array2map;
	exports.assign = assign;
	exports.assignIfUndef = assignIfUndef;
	exports.assignProp = assignProp;
	exports.assignPropIfUndef = assignPropIfUndef;
	exports.exist = exist;
	exports.get = get;
	exports.group = group;
	exports.map2array = map2array;
	exports.pick = pick;
	exports.put = put;
	exports.putIfUndef = putIfUndef;
	exports.putProp = putProp;
	exports.putPropIfUndef = putPropIfUndef;
	exports.redim = redim;
	exports.select = select;
	exports.set = set;
	exports.setIfUndef = setIfUndef;
	exports.setProp = setProp;
	exports.setPropIfUndef = setPropIfUndef;
	exports.traverse = traverse;
	exports.traverseReverse = traverseReverse;

	Object.defineProperty(exports, '__esModule', { value: true });

})));
