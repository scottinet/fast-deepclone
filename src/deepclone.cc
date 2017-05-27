#include <v8.h>
#include <node.h>
#include <nan.h>
#include <cstdint>
#include <unordered_map>
#include <iostream>

typedef v8::Local<v8::Object> v8LocalObject;
typedef std::unordered_map<int, v8LocalObject> circularMap;

bool isClonable(const v8::Local<v8::Value> & obj) {
  return obj->IsObject()
    && !obj->IsFunction()
    && !obj->IsNativeError()
    && !obj->IsArrayBuffer()
    && !obj->IsArrayBufferView()
    && !obj->IsDate()
    && !obj->IsBooleanObject()
    && !obj->IsNumberObject()
    && !obj->IsProxy()
    && !obj->IsRegExp()
    && !obj->IsSharedArrayBuffer()
    && !obj->IsSymbolObject()
    && !obj->IsPromise()
    && !obj->IsWeakMap()
    && !obj->IsMap()
    && !obj->IsSet()
    && !obj->IsWeakSet()
    && !obj->IsStringObject();
}

v8LocalObject clone(circularMap & refs, const v8LocalObject & source) {
  int uid = source->GetIdentityHash();
  auto circularReference = refs.find(uid);

  if (circularReference != refs.end()) {
    return circularReference->second;
  }

  const v8LocalObject target = source->Clone();
  const v8::Local<v8::Array> keys = Nan::GetOwnPropertyNames(source).ToLocalChecked();
  const unsigned length = keys->Length();

  if (length == 0) {
    return target;
  }

  refs[uid] = target;

  for(unsigned i = 0; i < length; i++) {
    v8::Local<v8::Value> key = keys->Get(i);
    v8::Local<v8::Value> val = Nan::Get(source, key).ToLocalChecked();

    if (isClonable(val)) {
      Nan::Set(target, key, clone(refs, val->ToObject()));
    }
    else if (val->IsRegExp()) {
      v8::Local<v8::RegExp> regexp = v8::Local<v8::RegExp>::Cast(val);
      Nan::Set(target, key, Nan::New<v8::RegExp>(regexp->GetSource(), regexp->GetFlags()).FromMaybe(val));
    }
  }

  return target;
}

NAN_METHOD(deepClone) {
  v8LocalObject source = info[0].As<v8::Object>();

  if (isClonable(source)) {
    circularMap refs;
    info.GetReturnValue().Set(clone(refs, source));
  }
  else {
    info.GetReturnValue().Set(source);
  }
}

NAN_MODULE_INIT(init) {
  Nan::Set(target, Nan::New("deepClone").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(deepClone)).ToLocalChecked());
}

NODE_MODULE(DeepClone, init)
