//
// Created by red on 10/1/17.
//

#ifndef NPDF_STREAM_H
#define NPDF_STREAM_H

#include <napi.h>
#include <podofo/podofo.h>
#include <string>

using namespace std;

namespace NoPoDoFo {
class Stream : public Napi::ObjectWrap<Stream>
{
public:
  explicit Stream(const Napi::CallbackInfo& callbackInfo);
  ~Stream();
  static Napi::FunctionReference constructor;
  static void Initialize(Napi::Env& env, Napi::Object& target);
  Napi::Value Write(const Napi::CallbackInfo&);
  Napi::Value GetBuffer(const Napi::CallbackInfo&);
  Napi::Value GetFilteredBuffer(const Napi::CallbackInfo&);
  PoDoFo::PdfStream* GetStream() { return stream; }

private:
  PoDoFo::PdfStream* stream;
};
}
#endif // NPDF_STREAM_H
