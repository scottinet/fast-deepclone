//eslint-disable no-console

const 
  Benchmark = require('benchmark'),
  _ = require('lodash'),
  clone = require('clone'),
  randomObject = require('random-object').randomObject,
  deepClone = require('.');

const suite = new Benchmark.Suite;
let obj;

suite
  .add('lodash.cloneDeep', () => {
    _.cloneDeep(obj);
  })
  .add('clone', () => {
    clone(obj);
  })
  .add('fast-deepclone', () => {
    deepClone(obj, {circular: false});
  })
  .on('cycle', event => {
    console.log(String(event.target));
  });

console.log('Cloning empty objects:');
obj = {};
suite.run({async: false });
console.log('===');

console.log('Cloning objects with 10 properties and a depth of 3:');
obj = randomObject(10, 3);
suite.run({async: false});
console.log('===');

console.log('Cloning objects with 100 properties and a depth of 100:');
obj = randomObject(100, 100);
suite.run({async: false});
console.log('===');
