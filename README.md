# fast-deepclone

Fast and safe JSON deep-cloning utility


## Behavior

POJOs and arrays are always duplicated. 

Circular references are carried over to the cloned object.

Objects properties and prototypes are ignored.

The resulting duplicated objects keep the same references to the following objects:

* functions and generator functions
* native Error objects
* WeakMap 
* WeakSet 
* Promise
* symbols
* object version of scalar values (e.g. BooleanObject)

This module also allows to either keep references or to duplicate the following objects, depending on the provided options:

* Buffer
* all TypedArrays
* Map
* Set
* Date 
* RegExp


## Installation

`npm fast-deepclone`

This module can only be used with NodeJS.


## How to use


```js
const deepClone = require('fast-deepclone');


/*
  Clone objects, keeping references to Buffer, TypedArrays,
  Map, Set, Date and RegExp.
  This means that modifying one of those objects in the
  source object will also impact the target object
 */
const clonedTarget = deepClone(source);
// or: const clonedTarget = deepClone(source, false);

/*
  Copy objects: new Buffer, TypedArrays, Map, Set, Date
  and RegExp objects are created in the target object,
  and their values are copied from the source
 */
const copiedTarget = deepClone(source, true);
```

## When to use this module

This module is written in C++, using V8 for faster object access.  
This is, to my knowledge, the fastest deep-cloning module currently available, but it can only work with NodeJS.

If you need a deep-cloning tool in browsers, check either [lodash.cloneDeep](https://www.npmjs.com/package/lodash) or [clone](https://www.npmjs.com/package/clone). These modules are well-written, well-tested, and they also handle circular references/JS Objects.

Moreover, if all you need to duplicate are POJO objects (i.e. objects containing only objects, arrays or scalar values), and if you are absolutely certain that there is no circular reference in them, then you may want to use `JSON.parse(JSON.stringify(<object_to_clone>))` instead.  
Duplicating objects using `JSON` functions is way faster. Around 2x to 6x faster than this module, depending on the nature of the duplicated object.  
This is because these functions are hardcoded within V8 and have access to internal structures, not exposed in the V8 public API. Which is not necessarily a bad thing, but a bit frustrating.


## Benchmarks

Obtained by running `node benchmark.js` at the root of the project:

```
== Empty objects
lodash.cloneDeep x 757,928 ops/sec ±0.36% (91 runs sampled)
clone x 543,766 ops/sec ±0.61% (95 runs sampled)
fast-deepclone (clone mode) x 1,918,976 ops/sec ±0.52% (93 runs sampled)
fast-deepclone (copy mode) x 1,870,139 ops/sec ±0.65% (94 runs sampled)

== Fast POJO objects (5 properties, depth === 3)
lodash.cloneDeep x 65,058 ops/sec ±0.76% (94 runs sampled)
clone x 63,001 ops/sec ±0.58% (94 runs sampled)
fast-deepclone (clone mode) x 75,846 ops/sec ±0.51% (95 runs sampled)
fast-deepclone (copy mode) x 75,718 ops/sec ±0.56% (95 runs sampled)

== Slow POJO objects (100 properties, depth === 100)
lodash.cloneDeep x 95.13 ops/sec ±1.49% (69 runs sampled)
clone x 115 ops/sec ±1.12% (80 runs sampled)
fast-deepclone (clone mode) x 310 ops/sec ±0.60% (88 runs sampled)
fast-deepclone (copy mode) x 306 ops/sec ±1.00% (88 runs sampled)

== Fast objects w/ constructors and circular refs
lodash.cloneDeep x 4,375 ops/sec ±1.64% (88 runs sampled)
clone x 7,238 ops/sec ±1.74% (90 runs sampled)
fast-deepclone (clone mode) x 59,371 ops/sec ±0.40% (94 runs sampled)
fast-deepclone (copy mode) x 45,069 ops/sec ±0.11% (94 runs sampled)
```

## License

[MIT](https://opensource.org/licenses/MIT)
