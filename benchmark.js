/* eslint-disable no-console */

const 
  Benchmark = require('benchmark'),
  _ = require('lodash'),
  clone = require('clone'),
  randomObject = require('random-object').randomObject,
  crypto = require('crypto'),
  deepClone = require('.');

const testSet = [
  {name: 'Empty objects', content: {}},
  {name: 'Fast POJO objects (5 properties, depth === 3)', content: randomObject(5, 3)},
  {name: 'Slow POJO objects (100 properties, depth === 100)', content: randomObject(100, 100)},
  {
    name: 'Fast objects w/ constructors and circular refs', 
    content: {
      foo: 'bar',
      baz: 'qux',
      obj1: {
        obj2: {
          some: 'value'
        }
      },
      nil: null,
      undef: undefined,
      arr: [{foo: 'bar'}, 'foo', 'bar', 'baz'],
      func: (param) => console.log(param),
      err: new Error('error'),
      promise: new Promise(() => {}),
      wmap: new WeakMap(),
      map: new Map([['foo', 'bar'], ['baz', 'qux']]),
      set: new Set(['foo', 'bar', 'baz', 'qux']),
      wset: new WeakSet(),
      strobj: new String('foobar'), // eslint-disable-line
      buffer: crypto.randomBytes(26),
      float32: new Float32Array(crypto.randomBytes(32)),
      float64: new Float64Array(crypto.randomBytes(14)),
      int32: new Int32Array(crypto.randomBytes(24)),
      int16: new Int16Array(crypto.randomBytes(23)),
      int8: new Int8Array(crypto.randomBytes(12)),
      uint8: new Uint8Array(crypto.randomBytes(17)),
      uint8clamped: new Uint8ClampedArray(crypto.randomBytes(15)),
      date: new Date(),
      regex: new RegExp('/foobar/g')
    }
  }
];

// Adding circular references to the testSet
testSet[3].content.obj1.obj2.circular1 = testSet[3].content;
testSet[3].content.circular2 = testSet[3].content.obj1;

const suite = new Benchmark.Suite;
let obj;

suite
  .add('lodash.cloneDeep', () => {
    _.cloneDeep(obj);
  })
  .add('clone', () => {
    clone(obj);
  })
  .add('fast-deepclone (clone mode)', () => {
    deepClone(obj);
  })
  .add('fast-deepclone (copy mode)', () => {
    deepClone(obj, true);
  })
  .on('cycle', event => {
    console.log(String(event.target));
  });

for(let i = 0; i < testSet.length; i++) {
  console.log(`== ${testSet[i].name}`);
  obj = testSet[i].content;
  suite.run({async: false});
  console.log('');
}
