[![Build Status](https://travis-ci.org/scottinet/fast-deepclone.svg?branch=master)](https://travis-ci.org/scottinet/fast-deepclone)

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
* TypedArrays
* Map
* Set
* Date 
* RegExp


## Installation

`npm fast-deepclone`

This module can only be used with NodeJS (tested on all major versions from v4 to v8 included).


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
This is because these functions are hardcoded within V8 and have access to internal structures, not exposed in the V8 public API. Which is not necessarily a bad thing, if a bit frustrating.


## Benchmarks

Obtained by running `node benchmark.js` at the root of the project, using NodeJS v10.9.0:

```
== Empty objects
lodash.cloneDeep x 2,518,033 ops/sec ±1.44% (92 runs sampled)
clone x 2,836,807 ops/sec ±2.49% (89 runs sampled)
fast-deepclone (clone mode) x 3,701,173 ops/sec ±0.70% (94 runs sampled)
fast-deepclone (copy mode) x 3,517,442 ops/sec ±0.64% (90 runs sampled)

== Fast POJO objects (5 properties, depth === 3)
lodash.cloneDeep x 206,573 ops/sec ±0.66% (98 runs sampled)
clone x 249,522 ops/sec ±1.45% (92 runs sampled)
fast-deepclone (clone mode) x 133,702 ops/sec ±0.68% (94 runs sampled)
fast-deepclone (copy mode) x 129,638 ops/sec ±0.59% (95 runs sampled)

== Slow POJO objects (100 properties, depth === 100)
lodash.cloneDeep x 254 ops/sec ±1.19% (84 runs sampled)
clone x 297 ops/sec ±1.51% (87 runs sampled)
fast-deepclone (clone mode) x 519 ops/sec ±0.51% (91 runs sampled)
fast-deepclone (copy mode) x 510 ops/sec ±0.73% (90 runs sampled)

== Fast objects w/ constructors and circular refs
lodash.cloneDeep x 15,130 ops/sec ±1.02% (91 runs sampled)
clone x 15,333 ops/sec ±19.09% (76 runs sampled)
fast-deepclone (clone mode) x 87,303 ops/sec ±0.24% (95 runs sampled)
fast-deepclone (copy mode) x 45,266 ops/sec ±0.88% (91 runs sampled)

```

## License

[MIT](https://opensource.org/licenses/MIT)
