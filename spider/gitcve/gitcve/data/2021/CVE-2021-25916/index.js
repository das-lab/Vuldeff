'use strict';

const isObject = element =>
  typeof element === 'object';

const noObjectsInArray = array =>
  array.findIndex(isObject) === -1;

const arrayMerge = (original, patch, removeMode) => {
  let orig_length = original.length;
  const patch_length = patch.length;
  let i, isArr;

  if(patch_length < orig_length) {
    throw new Error('Cannot remove object array elements, only make them null');
  }

  //merge each array item
  for(i = 0; i < orig_length; i++) {
    if(
        //replace simple data types
        !isObject(patch[i]) ||

        //replace with null
        patch[i] === null ||

        (
          (!isObject(original[i]) || original[i] === null) &&
          //empty patch object means keep the original as it is
          Object.getOwnPropertyNames(patch[i]).length
        ) ||

        (
          //replace object with array or array with object
          (isArr = Array.isArray(patch[i])) !==
          Array.isArray(original[i])
        ) ||

        (
          //replace arrays containing only simple data types
          isArr &&
          noObjectsInArray(patch[i]) &&
          noObjectsInArray(original[i])
        )
    ) {
      original[i] = patch[i];
    }
    else if(isArr) {
      original[i] = arrayMerge(original[i], patch[i], removeMode);
    }
    else {
      original[i] = objectMerge(original[i], patch[i], removeMode);
    }
  }

  //just append new elements to the end of the original array
  while(orig_length !== patch_length) {
    original.push(patch[orig_length++]);
  }

  return original;
}

const objectMerge = (original, patch, removeMode) => {
  const patchProps = Object.getOwnPropertyNames(patch);
  let i, len;

  for(i = 0, len = patchProps.length; i < len; i++) {
    const name = patchProps[i];
    let isArr;

    if(
        //replace simple data types
        !isObject(patch[name]) ||

        //replace with null
        patch[name] === null ||

        (
          //replace object with array or array with object
          (isArr = Array.isArray(patch[name])) !==
          Array.isArray(original[name])
        ) ||

        (
          //replace arrays containing only simple data types
          isArr &&
          noObjectsInArray(patch[name]) &&
          noObjectsInArray(original[name])
        )
    ) {
      if(removeMode !== 1 || patch[name] !== null) {
        original[name] = patch[name];
      }
      else {
        original[name] = undefined;
      }
    }
    //merge arrays containing objects
    else if(isArr) {
      original[name] = arrayMerge(original[name], patch[name], removeMode);
    }
    //merge objects
    else {
      original[name] = objectMerge(original[name], patch[name], removeMode);
    }
  }

  return original;
};

module.exports = (original, patch, removeMode) =>
  //removeMode: what to do with fields in the patch that are null or an empty object
  // 0 - null sets properties to null
  // 1 - null removes properties, (similar to RFC 7396)

  //in an array containing objects, empty object leaves an element as it is
  //arrays that don't contain objects are replaced unitarily

  //deep clone the original and patch objects.
  //We want to throw an error if there are circular references (
  //which would greatly complicate the merge algorithm)
  patch === null?null:objectMerge(
    JSON.parse(JSON.stringify(original)),
    JSON.parse(JSON.stringify(patch)),
    (typeof removeMode !== 'number') ? 1: removeMode
  );
