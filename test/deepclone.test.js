const 
  assert = require('assert'),
  crypto = require('crypto'),
  deepclone = require('..');

describe('deepclone unit tests', () => {
  it('should clone a POJO object', () => {
    const src = {
      foo: 'bar',
      bar: {
        baz: {
          qux: 'qux'
        }
      },
      qux: [1, 'foo', 3.14, {bar: 'baz'}]
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

  it('should return the provided argument if it is not an object', () => {
    for (const arg of [undefined, null, 3.14, 123, 'foobar']) {
      assert.strictEqual(arg, deepclone(arg));
    }
  });

  it('scalar values', () => {
    const src = {
      str: 'foobar',
      int: 123,
      float: 3.14,
      bool: false,
      nil: null,
      undef: undefined
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);

    for (const key of Object.keys(src)) {
      assert.strictEqual(src[key], cloned[key]);
    }
  });

  it('clone/copy: Map', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: new Map([[1, 2], [2, 3], [3, 4]])
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual([...src.baz.entries()], [...copied.baz.entries()]);

  });

  it('clone/copy: Set', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: new Set([1, 2, 3, 4, 5])
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual([...src.baz.entries()], [...copied.baz.entries()]);
  });

  it('clone/copy: Buffer', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: crypto.randomBytes(12)
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual([...src.baz.entries()], [...copied.baz.entries()]);
  });

  it('clone/copy: Float64Array', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: new Float64Array(crypto.randomBytes(12))
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual([...src.baz.entries()], [...copied.baz.entries()]);
  });

  it('clone/copy: Float32Array', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: new Float32Array(crypto.randomBytes(12))
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual([...src.baz.entries()], [...copied.baz.entries()]);
  });

  it('clone/copy: Int32Array', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: new Int32Array(crypto.randomBytes(12))
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual([...src.baz.entries()], [...copied.baz.entries()]);
  });

  it('clone/copy: Int16Array', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: new Int16Array(crypto.randomBytes(12))
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual([...src.baz.entries()], [...copied.baz.entries()]);
  });

  it('clone/copy: Int8Array', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: new Int8Array(crypto.randomBytes(12))
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual([...src.baz.entries()], [...copied.baz.entries()]);
  });

  it('clone/copy: Uint8Array', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: new Uint8Array(crypto.randomBytes(12))
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual([...src.baz.entries()], [...copied.baz.entries()]);
  });

  it('clone/copy: Uint8ClampedArray', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: new Uint8ClampedArray(crypto.randomBytes(12))
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual([...src.baz.entries()], [...copied.baz.entries()]);
  });

  it('clone/copy: Date', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: new Date()
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual(src.baz.valueOf(), copied.baz.valueOf());
  });

  it('clone/copy: RegExp', () => {
    const src = {
      foo: 'bar',
      bar: 123,
      baz: /^foobar/ig
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);
    assert.strictEqual(src.baz, cloned.baz);

    const copied = deepclone(src, true);

    assert.deepEqual(src, copied);
    assert.notStrictEqual(src.baz, copied.baz);
    assert.deepEqual(src.baz.toString(), copied.baz.toString());
  });

  it('clone: uncopiable objects', () => {
    const src = {
      wmap: new WeakMap(),
      wset: new WeakSet(),
      string: String('foobar'),
      number: Number(123),
      boolean: Boolean(true),
      error: new Error('foobar'),
      func: () => {},
      generator: function *gen() {},
      sym: Symbol()
    };

    const cloned = deepclone(src);

    assert.deepEqual(src, cloned);

    for (const key of Object.keys(src)) {
      assert.strictEqual(src[key], cloned[key]);
    }
  });
});
