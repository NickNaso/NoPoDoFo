//
// Created by red on 9/10/17.
//

#ifndef NPDF_PDFFIELD_H
#define NPDF_PDFFIELD_H

#include <napi.h>
#include <podofo/podofo.h>
#include <string>
#include "ValidateArguments.h"

using namespace Napi;
using namespace std;
using namespace PoDoFo;

class Field : public Napi::ObjectWrap<Field>
{
public:
  Field(const CallbackInfo &info);
  ~Field() { delete _field; }
  static FunctionReference constructor;
  static void
  Initialize(Napi::Env &env, Napi::Object &target)
  {
    HandleScope scope(env);
    Function ctor = DefineClass(env,
                                "Field",
                                {InstanceMethod("getType", &Field::GetType),
                                 InstanceMethod("getFieldName", &Field::GetFieldName),
                                 InstanceMethod("getAlternateName", &Field::GetAlternateName),
                                 InstanceMethod("getMappingName", &Field::GetMappingName),
                                 InstanceMethod("setAlternateName", &Field::SetAlternateName),
                                 InstanceMethod("setMappingName", &Field::SetMappingName),
                                 InstanceMethod("setRequired", &Field::SetRequired),
                                 InstanceMethod("isRequired", &Field::IsRequired)});
    constructor = Napi::Persistent(ctor);
    constructor.SuppressDestruct();
    target.Set("Field", constructor);
  }

  Napi::Value
  GetType(const CallbackInfo &);
  Napi::Value
  GetFieldName(const CallbackInfo &);
  Napi::Value
  GetAlternateName(const CallbackInfo &);
  Napi::Value
  GetMappingName(const CallbackInfo &);
  void
  SetAlternateName(const CallbackInfo &);
  void
  SetMappingName(const CallbackInfo &);
  void
  SetRequired(const CallbackInfo &);
  Napi::Value
  IsRequired(const CallbackInfo &);

private:
  PdfField *_field;
};

#endif //NPDF_PDFFIELD_H
