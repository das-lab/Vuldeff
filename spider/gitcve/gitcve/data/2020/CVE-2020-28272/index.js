exports.get = getByPath;
exports.set = setByPath;
exports.has = hasPath;
exports.push = pushByPath;
exports.method = methodByPath;
exports.call = callByPath;
exports.breadcrumbs = breadcrumbs;
exports.select = select;
exports.at = at;
exports.structure = getStructure;

/**
 * Property path as String delimited with dots or array of keys and indexes.
 * @typedef {String|Array<String|Number} Path
 */

/**
 * select - Create breadcrumbs object without the root element.
 *
 * @param  {*} target Nested object or anything else.
 * @param  {Path} path Path to select.
 * @return {*[]} Array of values for each path segment.
 */
function select(target, path) {
  return breadcrumbs(target, path).slice(1);
}

/**
 * breadcrumbs - Extract nested value by path and return as array. If target is not an object
 * or path is empty returns empty array.
 *
 * @param  {*} target Value.
 * @param  {Path} path   Path to value.
 * @return {*[]}        Values for path components.
 * @example
 *
 * breadcrumbs({a: b: {1}}, ['a', 'b']); // -> [{b:1}, 1];
 */
function breadcrumbs(target, path) {
  path = pathToArray(path);

  const result = [target];
  let part;
  let value = target;

  if (! isObject(value)) {
    return result;
  }

  for (let i = 0, l = path.length; i < l; i++) {
    part = path[i];

    if (! value.hasOwnProperty(part)) {
      break;
    }

    result.push(value[part]);

    value = value[part];
  }

  return result;
}

/**
 * hasPath - Set deeply nested value into target object. If nested properties
 * are not an objects or not exists creates them.
 *
 * @param {Object} target Parent object.
 * @param {Path} path   Array of path segments.
 * @return {Boolean} Returns true if object contains `path`.
 */
function hasPath(target, path) {
  path = pathToArray(path);

  const result = breadcrumbs(target, path);

  return result.length === path.length + 1;
}

/**
 * setByPath - Set deeply nested value into target object. If nested properties
 * are not an objects or not exists creates them.
 *
 * @param {Object} target Parent object.
 * @param {Path} path Array of path segments.
 * @param {*} value Value to set.
 * @return {void}
 */
function setByPath(target, path, value) {
  path = pathToArray(path);

  if (! path.length) {
    return value;
  }

  const key = path[0];
  if (isNumber(key)) {
    if (! Array.isArray(target)) {
      target = [];
    }
  }
  else if (! isObject(target)) {
    target = {};
  }

  if (path.length > 1) {
    target[key] = setByPath(target[key], path.slice(1), value);
  }
  else {
    target[key] = value;
  }

  return target;
}

/**
 * Push deeply nested value into target object. If nested properties are not an
 * objects or not exists creates them.
 *
 * @param {*} target Parent object.
 * @param {Path} path   Array of path segments.
 * @param {*} value Value to set.
 * @return {Object|Array} Returns updated `target`.
 */
function pushByPath(target, path, value) {
  path = pathToArray(path);

  if (! path.length) {
    if (! Array.isArray(target)) {
      return [value];
    }
    else {
      target.push(value);
      return target;
    }
  }

  if (! isObject(target)) {
    target = {};
  }

  return at(target, path, function(finalTarget, key) {
    if (Array.isArray(finalTarget[key])) {
      finalTarget[key].push(value);
    }
    else {
      finalTarget[key] = [value];
    }

    return finalTarget;
  });
}

/**
 * @callback atCallback
 * @param {*} target Target value.
 * @param {?(String|Number)} key Key or index of property.
 * @return {*} Return new target valus.
 */

/**
 * at - Update nested `target` at `path` using `update` function.
 *
 * @param  {*} target Nest object or anything else.
 * @param  {Path} path Property path.
 * @param  {atCallback} update Update last path's item.
 * @return {*} Updated target value
 */
function at(target, path, update) {
  path = pathToArray(path);

  if (! path.length) {
    return update(target, null);
  }

  const key = path[0];
  if (isNumber(key)) {
    if (! Array.isArray(target)) {
      target = [];
    }
  }
  else if (! isObject(target)) {
    target = {};
  }

  if (path.length > 1) {
    target[key] = at(target[key], path.slice(1), update);
  }
  else {
    target = update(target, key);
  }

  return target;
}

/**
 * getByPath - Get value from `target` by `path`.
 *
 * @param  {*} target Nested object or anything else.
 * @param  {Path} path Path to nested property.
 * @return {*} Returns value or undefined.
 */
function getByPath(target, path) {
  path = pathToArray(path);

  const values = breadcrumbs(target, path);

  return values[values.length - 1];
}

/**
 * methodByPath - Receive method from deeply nested object as function with
 * captured context as the method's owner object.
 *
 * @param  {*} target Deeply nested object or anything else.
 * @param  {Path} path Path to the method.
 * @return {Function} Returns function.
 */
function methodByPath(target, path) {
  path = pathToArray(path);

  const values = breadcrumbs(target, path);

  if (values.length < path.length) {
    return noop;
  }

  if (typeof values[values.length - 1] !== 'function') {
    return noop;
  }

  if (values.length > 1) {
    return values[values.length - 1].bind(values[values.length - 2]);
  }
  else {
    return values[0].bind(target);
  }
}

/**
 * callByPath - Call method by it's path in nested object.
 *
 * @param  {*} target Nested object or anything else.
 * @param  {Path} path Path to nested property.
 * @param  {*[]} args Arguments of function call.
 * @return {*} Result of function call or undefined if method not exists.
 */
function callByPath(target, path, args) {
  var fn = methodByPath(target, path);
  if (! fn) {
    return;
  }

  return fn.apply(null, args);
}

function getStructure(target, prefix) {
  if (! isObject(target)) {
    return [{path: [], value: target}];
  }

  if (! prefix) {
    prefix = [];
  }

  if (Array.isArray(target)) {
    return target.reduce(function (result, value, i) {
      return result.concat(
        getPropStructure(value, prefix.concat(i))
      );
    }, []);
  }
  else {
    return Object.getOwnPropertyNames(target)
    .reduce(function(result, key) {
      const value = target[key];

      return result.concat(
        getPropStructure(value, prefix.concat(key))
      );
    }, []);
  }
}

function getPropStructure(value, path) {
  if (isObject(value)) {
    return getStructure(value, path);
  }
  else {
    return [{
      path: path,
      value: value,
    }];
  }
}

function isObject(target) {
  return target !== null && typeof target === 'object';
}

function isNumber(target) {
  return typeof target === 'number';
}

function noop() {}

function pathToArray(path) {
  if (typeof path === 'string') {
    if (path.length) {
      return path.split('.');
    }
    else {
      return [];
    }
  }
  else {
    return path;
  }
}
