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
    const keys = Object.keys(target);
    if (Object.getOwnPropertySymbols) {
        const symbols = Object.getOwnPropertySymbols(target).filter(symbol => {
            const descriptor = Object.getOwnPropertyDescriptor(target, symbol);
            return descriptor && descriptor.enumerable;
        });
        if (symbols.length) {
            keys.push(...symbols);
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
    const { name, getName } = descriptor;
    if (name !== undefined) {
        return name;
    }
    if (getName) {
        return getName.call(current, current);
    }
}
function getNonEmptyPropName(current, descriptor) {
    const name = getPropName(current, descriptor);
    return name !== undefined ? name : 'undefined';
}
function getPropNames(current, descriptor) {
    const { names, getNames } = descriptor;
    if (names !== undefined) {
        return isArray(names) ? names : [names];
    }
    if (getNames) {
        const gotNames = getNames.call(current, current);
        if (gotNames !== undefined) {
            return isArray(gotNames) ? gotNames : [gotNames];
        }
    }
    return getOwnEnumerablePropKeys(current);
}

function generate(target, hierarchies, forceOverride) {
    let current = target;
    hierarchies.forEach(info => {
        const descriptor = normalizeDescriptor(info);
        const { value, type, create, override, created, skipped, got } = descriptor;
        const name = getNonEmptyPropName(current, descriptor);
        if (forceOverride || override || !current[name] || typeof current[name] !== 'object') {
            const obj = value ? value :
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
        const parent = current;
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
    const current = generate(target, hierarchies.slice(0, -1));
    const last = generate(current, hierarchies.slice(-1), true);
    return { current, last };
}

function _parseArgs(others) {
    const value = others[others.length - 1];
    const rest = Array.prototype.concat.apply([], others.slice(0, -1)); // exclude `value`
    const hierarchies = rest.slice(0, -1);
    const prop = rest[rest.length - 1];
    return { hierarchies, prop, value };
}
function set(optionalTarget, ...others) {
    const { hierarchies, prop, value } = _parseArgs(others);
    const target = optionalTarget || {};
    const current = setupIfUndef(target, hierarchies);
    current[prop] = value;
    return target;
}
function assign(target, ...others) {
    const { hierarchies, prop, value } = _parseArgs(others);
    const current = setupIfUndef(target, hierarchies);
    current[prop] = value;
    return current;
}
function put(target, ...others) {
    const { hierarchies, prop, value } = _parseArgs(others);
    const current = setupIfUndef(target, hierarchies);
    current[prop] = value;
    return value;
}
function setIfUndef(optionalTarget, ...others) {
    const { hierarchies, prop, value } = _parseArgs(others);
    const target = optionalTarget || {};
    const current = setupIfUndef(target, hierarchies);
    if (current[prop] === undefined) {
        current[prop] = value;
    }
    return target;
}
function assignIfUndef(target, ...others) {
    const { hierarchies, prop, value } = _parseArgs(others);
    const current = setupIfUndef(target, hierarchies);
    if (current[prop] === undefined) {
        current[prop] = value;
    }
    return current;
}
function putIfUndef(target, ...others) {
    const { hierarchies, prop, value } = _parseArgs(others);
    const current = setupIfUndef(target, hierarchies);
    if (current[prop] === undefined) {
        current[prop] = value;
    }
    return current[prop];
}

function _normalizeHierarchies(hierarchies) {
    const result = Array.prototype.concat.apply([], hierarchies);
    return result;
}
function setProp(optionalTarget, ...hierarchies) {
    const arrHierarchies = _normalizeHierarchies(hierarchies);
    const target = optionalTarget || {};
    setup(target, arrHierarchies);
    return target;
}
function assignProp(target, ...hierarchies) {
    const arrHierarchies = _normalizeHierarchies(hierarchies);
    const { current } = setup(target, arrHierarchies);
    return current;
}
function putProp(target, ...hierarchies) {
    const arrHierarchies = _normalizeHierarchies(hierarchies);
    const { last } = setup(target, arrHierarchies);
    return last;
}
function setPropIfUndef(optionalTarget, ...hierarchies) {
    const arrHierarchies = _normalizeHierarchies(hierarchies);
    const target = optionalTarget || {};
    setupIfUndef(target, arrHierarchies);
    return target;
}
function assignPropIfUndef(target, ...hierarchies) {
    const arrHierarchies = _normalizeHierarchies(hierarchies);
    const current = setupIfUndef(target, arrHierarchies.slice(0, -1));
    setupIfUndef(current, arrHierarchies.slice(-1));
    return current;
}
function putPropIfUndef(target, ...hierarchies) {
    const arrHierarchies = _normalizeHierarchies(hierarchies);
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
    const { getValue } = descriptor;
    let name = getPropName(current, descriptor);
    let value;
    if (name !== undefined) {
        value = current[name];
    }
    else {
        name = 'undefined';
        if (getValue) {
            value = getValue.call(current, current);
        }
    }
    const { got } = descriptor;
    if (got) {
        got.call(current, current, name, value);
    }
    return { name, value };
}

function get(target, ...rest) {
    const hierarchies = Array.prototype.concat.apply([], rest);
    let current = target;
    if (current !== undefined && current !== null) {
        hierarchies.every(info => {
            const descriptor = normalizeDescriptor$1(info);
            const { value } = getNameValue(current, descriptor);
            current = value;
            return current;
        });
    }
    return current;
}

function exist(target, ...rest) {
    if (target === undefined || target === null) {
        return false;
    }
    const hierarchies = Array.prototype.concat.apply([], rest);
    let current = target;
    for (let i = 0; i < hierarchies.length; i++) {
        const prop = hierarchies[i];
        if (!current || !(prop in current)) {
            return false;
        }
        current = current[prop];
    }
    return true;
}

function _parseArgs$1(others) {
    const callback = others[others.length - 1];
    const hierarchies = Array.prototype.concat.apply([], others.slice(0, -1)); // exclude `callback`
    return { hierarchies, callback };
}
function traverse(target, ...others) {
    const { hierarchies, callback } = _parseArgs$1(others);
    let current = target;
    if (current !== undefined && current !== null) {
        hierarchies.every(info => {
            const descriptor = normalizeDescriptor$1(info);
            const { name, value } = getNameValue(current, descriptor);
            const parent = current;
            current = value;
            const result = callback.call(parent, parent, name, current);
            return result !== false;
        });
    }
}
function traverseReverse(target, ...others) {
    const { hierarchies, callback } = _parseArgs$1(others);
    let current = target;
    if (current !== undefined && current !== null) {
        const params = [];
        hierarchies.every(info => {
            const descriptor = normalizeDescriptor$1(info);
            const { name, value } = getNameValue(current, descriptor);
            const parent = current;
            current = value;
            params.push({ parent, name, current });
            return current;
        });
        for (let i = params.length - 1; i >= 0; i--) {
            const item = params[i];
            const result = callback.call(item.parent, item.parent, item.name, item.current);
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
    const result = {};
    for (let i = 0; i < arr.length; i++) {
        const item = arr[i];
        const keyProp = get(item, key);
        const valueProp = get(item, value);
        result[keyProp] = valueProp;
    }
    return result;
}

function map2array(obj, keyName, valueName) {
    if (!obj) {
        return;
    }
    const result = [];
    getOwnEnumerablePropKeys(obj).forEach(key => {
        const value = obj[key];
        const keyProp = typeof keyName === 'function' ? keyName.call(obj, obj, key, value) : keyName;
        const valueProp = typeof valueName === 'function' ? valueName.call(obj, obj, key, value) : valueName;
        result.push({
            [keyProp]: key,
            [valueProp]: value
        });
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
    const { got, mapName, mapValue, mapped } = descriptor;
    const next = current[name];
    if (got) {
        got.call(current, current, name, next);
    }
    const mappedName = mapName ? mapName.call(current, current, name, next) : name;
    const mappedValue = mapValue ? mapValue.call(current, current, name, next) : next;
    if (mapped) {
        mapped.call(current, current, mappedName, mappedValue);
    }
    return { mappedName, mappedValue };
}

function generate$1(current, result, hierarchies, index) {
    const descriptor = normalizeDescriptor$2(hierarchies[index]);
    const names = getPropNames(current, descriptor);
    const lastIndex = hierarchies.length - 1;
    names.forEach(name => {
        if (name in current) {
            const { mappedName, mappedValue } = getMappedNameValue(current, name, descriptor);
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
function select(target, ...hierarchyProps) {
    let result;
    const current = target;
    if (current !== undefined && current !== null) {
        result = cloneContainer(current);
        generate$1(current, result, hierarchyProps, 0);
    }
    return result;
}

function find(current, result, hierarchies, index) {
    const descriptor = normalizeDescriptor$2(hierarchies[index]);
    const names = getPropNames(current, descriptor);
    const lastIndex = hierarchies.length - 1;
    names.forEach(name => {
        if (name in current) {
            const { mappedValue } = getMappedNameValue(current, name, descriptor);
            if (index < lastIndex) {
                find(mappedValue, result, hierarchies, index + 1);
            }
            else {
                result.push(mappedValue);
            }
        }
    });
}
function pick(target, ...hierarchyProps) {
    const result = [];
    const current = target;
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
    const { type, create } = descriptor;
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
function group(target, ...params) {
    if (!params.length) {
        return target;
    }
    const descriptors = Array.prototype.concat.apply([], params).map(normalizeDescriptor$3).filter(d => d.by);
    if (!descriptors.length) {
        return target;
    }
    const lastIndex = descriptors.length - 1;
    const keys = getOwnEnumerablePropKeys(target);
    let rootContainer;
    keys.forEach(key => {
        const child = target[key];
        let prevContainer;
        let prevName;
        descriptors.forEach((descriptor, index) => {
            const { by } = descriptor;
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
            const groupName = by.call(target, target, key, child);
            if (index !== lastIndex) {
                prevName = groupName;
            }
            else {
                if (!prevContainer[groupName]) {
                    prevContainer[groupName] = cloneContainer(target);
                }
                const currentContainer = prevContainer[groupName];
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

function _getDimTypes(input, maxDim = 16) {
    const types = [];
    if (isObject(input)) {
        let type = isArray(input) ? Array : Object;
        let dimItems = [input];
        for (let iDim = 0; iDim <= maxDim; iDim++) {
            let nextType = Array;
            let nextDimItems = [];
            dimItems.forEach(dimItem => {
                getOwnEnumerablePropKeys(dimItem).forEach(key => {
                    const nextDimItem = dimItem[key];
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
                break;
            }
            type = nextType;
            dimItems = nextDimItems;
        }
    }
    return types;
}
function redim(data, ...newDimsOrder) {
    if (!data) {
        return data;
    }
    // newDims: new order of old dims
    const newDims = Array.prototype.concat.apply([], newDimsOrder);
    if (!newDims.length) {
        return data;
    }
    const oldDimMin = Math.min(...newDims);
    if (oldDimMin < 0) {
        return;
    }
    const oldDimMax = Math.max(...newDims);
    const newDimMax = newDims.length - 1;
    const dimTypes = _getDimTypes(data, oldDimMax);
    if (!dimTypes.length || oldDimMax >= dimTypes.length) {
        return;
    }
    const result = new dimTypes[newDims[0]];
    const _walk = function _walk(path, current, currentDim) {
        if (currentDim <= oldDimMax) {
            getOwnEnumerablePropKeys(current).forEach(key => {
                const nextDim = currentDim + 1;
                if (exist(current, key)) {
                    _walk(path.concat(key), current[key], nextDim);
                }
            });
        }
        else {
            const newHierarchyDescriptors = newDims.map(((oldDim, newDim) => {
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

export { array2map, assign, assignIfUndef, assignProp, assignPropIfUndef, exist, get, group, map2array, pick, put, putIfUndef, putProp, putPropIfUndef, redim, select, set, setIfUndef, setProp, setPropIfUndef, traverse, traverseReverse };
