const 
  assert = require('assert'),
  deepclone = require('..');

describe('deepclone unit tests', () => {
  it('should clone a POJO object', () => {
    const src = {
      foo: 'bar',
      bar: {
        baz: {
          qux: 'qux'
        }
      }
    };

    const target = deepclone(src);

    assert.deepEqual(src, target);
    assert.notStrictEqual(src, target);
    assert.notStrictEqual(src.bar, target.bar);
    assert.notStrictEqual(src.bar.baz, target.bar.baz);
  });

  it('should carry over circular references', () => {
    const src = {
      foo: 'bar',
      bar: {
      }
    };

    src.bar.baz = src;

    const target = deepclone(src);

    assert.deepEqual(src, target);
    assert.strictEqual(target.bar.baz, target);
    assert.notStrictEqual(target.bar.baz, src);
  });
});
