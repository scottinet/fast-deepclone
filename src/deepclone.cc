#include <unordered_map>
#include <v8.h>
#include <node.h>
#include <nan.h>
#include <iostream>

using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::Value;
using v8::Array;
using v8::RegExp;
using v8::String;

typedef std::unordered_map<int, Local<Object>> circularMap;

bool isClonable(const Local<Value> & obj) {
  return obj->IsObject()
    && !(
      obj->IsFunction()
      || obj->IsNativeError()
      || obj->IsArrayBuffer()
      || obj->IsArrayBufferView()
      || obj->IsDate()
      || obj->IsBooleanObject()
      || obj->IsNumberObject()
      || obj->IsProxy()
      || obj->IsRegExp()
      || obj->IsSharedArrayBuffer()
      || obj->IsSymbolObject()
      || obj->IsPromise()
      || obj->IsWeakMap()
      || obj->IsMap()
      || obj->IsSet()
      || obj->IsWeakSet()
      || obj->IsStringObject()
    );
}

Local<Object> cloneObject(circularMap & refs, const Local<Object> & source) {
  int uid = source->GetIdentityHash();
  auto circularReference = refs.find(uid);

  if (circularReference != refs.end()) {
    return circularReference->second;
  }

  const Local<Object> target = source->Clone();
  const Local<Array> keys = Nan::GetOwnPropertyNames(source).ToLocalChecked();
  const unsigned length = keys->Length();

  if (length == 0) {
    return target;
  }

  refs[uid] = target;

  for(unsigned i = 0; i < length; i++) {
    const Local<Value> key = keys->Get(i);
    const Local<Value> val = Nan::Get(target, key).ToLocalChecked();

    if (val->IsObject()) {
      if (val->IsRegExp()) {
        Local<RegExp> regexp = Local<RegExp>::Cast(val);
        Nan::Set(target, key, Nan::New<RegExp>(regexp->GetSource(), regexp->GetFlags()).FromMaybe(val));
      }
      else if (val->IsArrayBufferView()) {
        Local<v8::ArrayBufferView> bufferView = Local<v8::ArrayBufferView>::Cast(val);
        
        if (bufferView->HasBuffer()) {
          if (val->IsFloat32Array()) {
            Nan::Set(target, key, v8::Float32Array::New(bufferView->Buffer(), bufferView->ByteOffset(), bufferView->ByteLength() / 4));
          }
          else if (val->IsFloat64Array()) {
            Nan::Set(target, key, v8::Float64Array::New(bufferView->Buffer(), bufferView->ByteOffset(), bufferView->ByteLength() / 8)); 
          }
          else if (val->IsInt16Array()) {
            Nan::Set(target, key, v8::Int16Array::New(bufferView->Buffer(), bufferView->ByteOffset(), bufferView->ByteLength() / 2)); 
          }
          else {
            char *data = static_cast<char*>(bufferView->Buffer()->GetContents().Data()) + bufferView->ByteOffset();
            Nan::Set(target, key, Nan::CopyBuffer(data, bufferView->ByteLength()).ToLocalChecked());
          }
        }
      }
      else if (isClonable(val)) {
        Nan::Set(target, key, cloneObject(refs, val->ToObject()));
      }
    }
  }

  return target;
}

NAN_METHOD(deepClone) {
  const int argc = info.Length();

  if (argc == 0) {
    info.GetReturnValue().Set(Nan::New<Object>());
    return;
  }

  const Local<Value> source = info[0];

  if (isClonable(source)) {
    circularMap refs;
    info.GetReturnValue().Set(cloneObject(refs, source->ToObject()));
  }
  else {
    info.GetReturnValue().Set(source);
  }
}

NAN_MODULE_INIT(init) {
  Nan::Set(target, Nan::New("deepClone").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(deepClone)).ToLocalChecked());
}

NODE_MODULE(DeepClone, init)
