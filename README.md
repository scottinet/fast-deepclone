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

Obtained by running `node benchmark.js` at the root of the project, using NodeJS v7.10.0:

```
== Empty objects
lodash.cloneDeep x 1,267,300 ops/sec ±0.94% (87 runs sampled)
clone x 741,513 ops/sec ±0.58% (94 runs sampled)
fast-deepclone (clone mode) x 2,732,798 ops/sec ±0.54% (90 runs sampled)
fast-deepclone (copy mode) x 2,651,307 ops/sec ±0.38% (93 runs sampled)

== Fast POJO objects (5 properties, depth === 3)
lodash.cloneDeep x 82,981 ops/sec ±0.97% (92 runs sampled)
clone x 86,903 ops/sec ±0.35% (95 runs sampled)
fast-deepclone (clone mode) x 96,752 ops/sec ±0.53% (95 runs sampled)
fast-deepclone (copy mode) x 95,926 ops/sec ±0.61% (96 runs sampled)

== Slow POJO objects (100 properties, depth === 100)
lodash.cloneDeep x 101 ops/sec ±2.99% (72 runs sampled)
clone x 111 ops/sec ±2.89% (68 runs sampled)
fast-deepclone (clone mode) x 367 ops/sec ±0.78% (89 runs sampled)
fast-deepclone (copy mode) x 357 ops/sec ±0.49% (88 runs sampled)

== Fast objects w/ constructors and circular refs
lodash.cloneDeep x 7,494 ops/sec ±1.05% (95 runs sampled)
clone x 7,950 ops/sec ±3.35% (87 runs sampled)
fast-deepclone (clone mode) x 73,751 ops/sec ±0.44% (93 runs sampled)
fast-deepclone (copy mode) x 49,588 ops/sec ±0.21% (92 runs sampled)
```

Results obtained on fast POJO objects are a bit weird using NodeJS v8.0.0, I'll update the benchmarks once one or more minor versions of NodeJS v8 are released, as I don't think NodeJS v8 is stable yet.

## License

[MIT](https://opensource.org/licenses/MIT)
