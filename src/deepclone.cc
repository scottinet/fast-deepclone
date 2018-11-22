#include <unordered_map>
#include <v8.h>
#include <node.h>
#include <nan.h>
#include <cstring>

using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::Value;
using v8::Array;

typedef std::unordered_map<int, Local<Object>> circularMap;

bool isClonable(const Local<Value> obj) {
  return obj->IsObject()
    && !(
      obj->IsFunction()
      || obj->IsNativeError()
      || obj->IsArrayBuffer()
      || obj->IsArrayBufferView()
      || obj->IsDate()
      || obj->IsBooleanObject()
      || obj->IsNumberObject()
#if NODE_MAJOR_VERSION > 5
      || obj->IsProxy()
#endif
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

bool isClonableLight(const Local<Value> obj) {
  return obj->IsObject()
    && !(
      obj->IsFunction()
      || obj->IsNativeError()
      || obj->IsArrayBufferView()
      || obj->IsBooleanObject()
      || obj->IsNumberObject()
#if NODE_MAJOR_VERSION > 5
      || obj->IsProxy()
#endif
      || obj->IsSharedArrayBuffer()
      || obj->IsSymbolObject()
      || obj->IsPromise()
      || obj->IsWeakMap()
      || obj->IsWeakSet()
      || obj->IsStringObject()
      || obj->IsExternal()
    );
}

void copyProperties(const Local<Object> source, const Local<Object> target) {
  const Local<Array> keys = Nan::GetOwnPropertyNames(source).ToLocalChecked();
  const unsigned length = keys->Length();

  if (length == 0) {
    return;
  }

  for(unsigned i = 0; i < length; i++) {
    const Local<Value> key = keys->Get(i);
    const Local<Value> val = Nan::Get(source, key).ToLocalChecked();
    // TODO: Should we not copy properties that already exist on the target?
    Nan::Set(target, key, val);
  }
}

Local<Object> cloneObject(circularMap & refs, const Local<Object> source, bool copy);

Local<Object> cloneObjectToTarget(circularMap & refs, const Local<Object> source, bool copy, const Local<Object> target, int uid) {
  v8::Isolate *isolate = v8::Isolate::GetCurrent();
  const Local<Array> keys = Nan::GetOwnPropertyNames(target).ToLocalChecked();
  const unsigned length = keys->Length();

  if (length == 0) {
    return target;
  }

  refs[uid] = target;

  for(unsigned i = 0; i < length; i++) {

    const Local<Value> key = keys->Get(i);
    const Local<Value> val = Nan::Get(target, key).ToLocalChecked();

    if (!copy) {
      if (isClonable(val)) {
        Nan::Set(target, key, cloneObject(refs, val->ToObject(), copy));
      }
    }
    else if (val->IsObject()) {
      if (val->IsRegExp()) {
        Local<v8::RegExp> regexp = Local<v8::RegExp>::Cast(val);
        Nan::Set(target, key, Nan::New<v8::RegExp>(regexp->GetSource(), regexp->GetFlags()).FromMaybe(val));
      }
      else if (val->IsArrayBufferView()) {
        Local<v8::ArrayBufferView> bufferView = Local<v8::ArrayBufferView>::Cast(val);
        Local<v8::ArrayBuffer> targetAB = v8::ArrayBuffer::New(isolate, bufferView->ByteLength());
        bufferView->CopyContents(targetAB->GetContents().Data(), bufferView->ByteLength());

        if (val->IsFloat32Array()) {
          Nan::Set(target, key, v8::Float32Array::New(targetAB, 0, targetAB->ByteLength() / 4));
        }
        else if (val->IsFloat64Array()) {
          Nan::Set(target, key, v8::Float64Array::New(targetAB, 0, targetAB->ByteLength() / 8));
        }
        else if (val->IsInt32Array()) {
          Nan::Set(target, key, v8::Int32Array::New(targetAB, 0, targetAB->ByteLength() / 4));
        }
        else if (val->IsInt16Array()) {
          Nan::Set(target, key, v8::Int16Array::New(targetAB, 0, targetAB->ByteLength() / 2));
        }
        else if (val->IsInt8Array()) {
          Nan::Set(target, key, v8::Int8Array::New(targetAB, 0, targetAB->ByteLength()));
        }
        else if (val->IsUint32Array()) {
          Nan::Set(target, key, v8::Uint32Array::New(targetAB, 0, targetAB->ByteLength() / 4));
        }
        else if (val->IsUint16Array()) {
          Nan::Set(target, key, v8::Uint16Array::New(targetAB, 0, targetAB->ByteLength() / 2));
        }
        else if (val->IsUint8Array()) {
          Local<v8::Uint8Array> arr = v8::Uint8Array::New(targetAB, 0, targetAB->ByteLength());

          /*
            NodeJS Buffer objects are Uint8Array with little to no way to
            differentiate between these two classes.
            So in this case we simply copy over the prototype from the
            source object to the target one
           */
          arr->SetPrototype(Nan::GetCurrentContext(), val->ToObject()->GetPrototype());
          Nan::Set(target, key, arr);
        }
        else if (val->IsUint8ClampedArray()) {
          Nan::Set(target, key, v8::Uint8ClampedArray::New(targetAB, 0, targetAB->ByteLength()));
        }
      }
      else if (val->IsArrayBuffer()) {
        Local<v8::ArrayBuffer> buffer = Local<v8::ArrayBuffer>::Cast(val);
        Local<v8::ArrayBuffer> targetAB = v8::ArrayBuffer::New(isolate, buffer->ByteLength());
        memcpy(targetAB->GetContents().Data(), buffer->GetContents().Data(), buffer->ByteLength());
        Nan::Set(target, key, targetAB);
      }
      else if (val->IsDate()) {
        Local<v8::Date> date = Local<v8::Date>::Cast(val);
        Nan::Set(target, key, Nan::New<v8::Date>(date->ValueOf()).ToLocalChecked());
      }
      else if (val->IsMap()) {
        Local<Array> values = Local<v8::Map>::Cast(val)->AsArray();
        Local<v8::Context> context = isolate->GetCurrentContext();
        Local<v8::Map> targetMap = v8::Map::New(isolate);

        for(unsigned int i = 0; i < values->Length(); i += 2) {
          targetMap->Set(context, values->Get(i), values->Get(i+1));
        }

        targetMap->SetPrototype(Nan::GetCurrentContext(), val->ToObject()->GetPrototype());
        copyProperties(val->ToObject(), targetMap->ToObject());
        Nan::Set(target, key, cloneObjectToTarget(refs, val->ToObject(), copy, targetMap->ToObject(), uid));
      }
      else if (val->IsSet()) {
        Local<Array> values = Local<v8::Set>::Cast(val)->AsArray();
        Local<v8::Context> context = isolate->GetCurrentContext();
        Local<v8::Set> targetSet = v8::Set::New(isolate);

        for(unsigned int i = 0; i < values->Length(); i++) {
          targetSet->Add(context, values->Get(i));
        }

		targetSet->SetPrototype(Nan::GetCurrentContext(), val->ToObject()->GetPrototype());
        copyProperties(val->ToObject(), targetSet->ToObject());
        Nan::Set(target, key, cloneObjectToTarget(refs, val->ToObject(), copy, targetSet->ToObject(), uid));
      }
      else if (isClonableLight(val)) {
        Nan::Set(target, key, cloneObject(refs, val->ToObject(), copy));
      }
    }
  }

  return target;
}

Local<Object> cloneObject(circularMap & refs, const Local<Object> source, bool copy) {
  int uid = source->GetIdentityHash();
  auto circularReference = refs.find(uid);

  if (circularReference != refs.end()) {
    return circularReference->second;
  }

  const Local<Object> target = source->Clone();
  return cloneObjectToTarget(refs, source, copy, target, uid);
}

NAN_METHOD(deepClone) {
  const int argc = info.Length();

  if (argc == 0) {
    info.GetReturnValue().Set(Nan::New<Object>());
    return;
  }

  bool deepCopy = false;

  if (argc > 1) {
    Local<Value> optCopy = info[1];

    if (optCopy->IsBoolean()) {
      deepCopy = optCopy->ToBoolean()->Value();
    }
  }

  const Local<Value> source = info[0];

  if (isClonable(source)) {
    circularMap refs;
    info.GetReturnValue().Set(cloneObject(refs, source->ToObject(), deepCopy));
  }
  else {
    info.GetReturnValue().Set(source);
  }
}

NAN_MODULE_INIT(init) {
  Nan::Set(target, Nan::New("deepClone").ToLocalChecked(), Nan::GetFunction(Nan::New<v8::FunctionTemplate>(deepClone)).ToLocalChecked());
}

NODE_MODULE(DeepClone, init)
