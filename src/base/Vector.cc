//
// Created by red on 19/11/17.
//

#include "Vector.h"
#include "../ErrorHandler.h"
#include "../ValidateArguments.h"
#include "../doc/Document.h"
#include "Obj.h"

using namespace std;
using namespace Napi;
using namespace PoDoFo;

namespace NoPoDoFo {

FunctionReference Vector::constructor;

Vector::Vector(const Napi::CallbackInfo& info)
  : ObjectWrap(info)
{
  vector = new PdfVecObjects();
}

Vector::~Vector()
{
  HandleScope scope(Env());
  vector = nullptr;
  document = nullptr;
}

void
Vector::Initialize(Napi::Env& env, Napi::Object& target)
{
  HandleScope scope(env);
  Function ctor = DefineClass(env,
                              "Vector",
                              { InstanceAccessor("document",
                                                 &Vector::GetParentDocument,
                                                 &Vector::SetParentDocument) });
  constructor = Persistent(ctor);
  constructor.SuppressDestruct();
  target.Set("Vector", ctor);
}

Napi::Value
Vector::GetParentDocument(const Napi::CallbackInfo& info)
{
  PdfMemDocument* doc;
  doc = dynamic_cast<PdfMemDocument*>(vector->GetParentDocument());
  return Document::constructor.New(
    { External<PdfMemDocument>::New(info.Env(), doc) });
}
void
Vector::SetParentDocument(const Napi::CallbackInfo& info,
                          const Napi::Value& value)
{

  if (!value.IsObject() ||
      !value.As<Napi::Object>().InstanceOf(Document::constructor.Value())) {
    throw Error::New(info.Env(), "must be an instance of Document");
  }

  vector->SetParentDocument(
    Document::Unwrap(value.As<Object>())->GetDocument());
}
void
Vector::SetAutoDelete(const Napi::CallbackInfo& info)
{
  AssertFunctionArgs(info, 1, { napi_boolean });
  vector->SetAutoDelete(info[0].As<Boolean>());
}
void
Vector::SetCanReuseObjectNumber(const Napi::CallbackInfo& info,
                                const Napi::Value& value)
{
  if (!value.IsBoolean()) {
    throw Error::New(info.Env(), "value must be a boolean");
  }
  vector->SetCanReuseObjectNumbers(value.As<Boolean>());
}
Napi::Value
Vector::GetCanReuseObjectNumber(const Napi::CallbackInfo& info)
{
  return Boolean::New(info.Env(), vector->GetCanReuseObjectNumbers());
}
void
Vector::Clear(const Napi::CallbackInfo& info)
{
  try {
    vector->Clear();
  } catch (PdfError& err) {
    ErrorHandler(err, info);
  }
}
Napi::Value
Vector::GetSize(const Napi::CallbackInfo& info)
{
  return Number::New(info.Env(), vector->GetSize());
}
Napi::Value
Vector::GetObjectCount(const Napi::CallbackInfo& info)
{
  return Number::New(info.Env(), vector->GetObjectCount());
}
Napi::Value
Vector::GetObject(const Napi::CallbackInfo& info)
{
  AssertFunctionArgs(info, 1, { napi_number });
  const pdf_objnum i = info[0].As<Number>();
  const pdf_gennum g = static_cast<const pdf_gennum>(
    info.Length() == 2 && info[1].IsNumber() ? info[1].As<Number>() : 0);
  PdfReference ref(i, g);
  return Obj::constructor.New({ External<PdfObject>::New(
    info.Env(), vector->GetObject(ref), [](Napi::Env env, PdfObject* value) {
      HandleScope scope(env);
      delete value;
    }) });
}
Napi::Value
Vector::GetIndex(const Napi::CallbackInfo& info)
{
  throw Error::New(info.Env(), "unimplemented");
}
Napi::Value
Vector::CreateObject(const Napi::CallbackInfo& info)
{
  throw Error::New(info.Env(), "unimplemented");
}
Napi::Value
Vector::RemoveObject(const Napi::CallbackInfo& info)
{
  throw Error::New(info.Env(), "unimplemented");
}
Napi::Value
Vector::CollectGarbage(const Napi::CallbackInfo& info)
{
  throw Error::New(info.Env(), "unimplemented");
}
}
