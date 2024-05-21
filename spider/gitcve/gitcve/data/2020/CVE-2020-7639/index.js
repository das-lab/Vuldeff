/**
 * Set given `path`
 *
 * @param {Object} obj
 * @param {String} path
 * @param {Mixed} val
 * @return {Object}
 * @api public
 */

exports.set = function (obj, path, val) {
  var segs = path.split('.');
  var attr = segs.pop();
  var src = obj;

  for (var i = 0; i < segs.length; i++) {
    var seg = segs[i];
    obj[seg] = obj[seg] || {};
    obj = obj[seg];
  }

  obj[attr] = val;

  return src;
};

/**
 * Get given `path`
 *
 * @param {Object} obj
 * @param {String} path
 * @return {Mixed}
 * @api public
 */

exports.get = function (obj, path) {
  var segs = path.split('.');
  var attr = segs.pop();

  for (var i = 0; i < segs.length; i++) {
    var seg = segs[i];
    if (!obj[seg]) return;
    obj = obj[seg];
  }

  return obj[attr];
};

/**
 * Delete given `path`
 *
 * @param {Object} obj
 * @param {String} path
 * @return {Mixed}
 * @api public
 */

exports.delete = function (obj, path) {
  var segs = path.split('.');
  var attr = segs.pop();

  for (var i = 0; i < segs.length; i++) {
    var seg = segs[i];
    if (!obj[seg]) return;
    obj = obj[seg];
  }

  if (Array.isArray(obj)) {
    obj.splice(path, 1);
  } else {
    delete obj[attr];
  }
};
