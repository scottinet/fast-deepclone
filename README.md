# fast-deepclone
Fast and safe JSON deep-cloning utility 

Work in progress: already the fastest deep-cloning library out there, but I won't be satisfied until I figure out exactly how V8 works. Getting there though.

# Benchmark results

Obtained by running `node benchmark.js` at the root of the project:

```
Cloning: empty objects
lodash.cloneDeep x 757,928 ops/sec ±0.36% (91 runs sampled)
clone x 543,766 ops/sec ±0.61% (95 runs sampled)
fast-deepclone (clone mode) x 1,918,976 ops/sec ±0.52% (93 runs sampled)
fast-deepclone (copy mode) x 1,870,139 ops/sec ±0.65% (94 runs sampled)
===
Cloning: simple POJO objects (10 properties, depth === 3)
lodash.cloneDeep x 30,894 ops/sec ±0.86% (93 runs sampled)
clone x 36,147 ops/sec ±0.54% (95 runs sampled)
fast-deepclone (clone mode) x 40,602 ops/sec ±0.51% (95 runs sampled)
fast-deepclone (copy mode) x 40,567 ops/sec ±0.50% (96 runs sampled)
===
Cloning: large-depth POJO objects (depth === 100)
lodash.cloneDeep x 95.13 ops/sec ±1.49% (69 runs sampled)
clone x 115 ops/sec ±1.12% (80 runs sampled)
fast-deepclone (clone mode) x 310 ops/sec ±0.60% (88 runs sampled)
fast-deepclone (copy mode) x 306 ops/sec ±1.00% (88 runs sampled)
===
Cloning: objects w/ constructors and circular refs
lodash.cloneDeep x 4,375 ops/sec ±1.64% (88 runs sampled)
clone x 7,238 ops/sec ±1.74% (90 runs sampled)
fast-deepclone (clone mode) x 59,371 ops/sec ±0.40% (94 runs sampled)
fast-deepclone (copy mode) x 45,069 ops/sec ±0.11% (94 runs sampled)
```
