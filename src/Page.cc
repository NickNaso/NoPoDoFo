//
// Created by red on 9/7/17.
//

#include "Page.h"
#include "Field.h"

Napi::FunctionReference Page::constructor;

Page::Page(const CallbackInfo& info)
  : ObjectWrap(info)
{
  PoDoFo::PdfPage* page = info[0].As<Napi::External<PoDoFo::PdfPage>>().Data();
  PoDoFo::PdfMemDocument* parent =
    info[1].As<Napi::External<PoDoFo::PdfMemDocument>>().Data();
  _parent = parent;
  _page = page;
  pageNumber = info[2].As<Number>();
}

Napi::Value
Page::GetRotation(const CallbackInfo& info)
{
  return Napi::Number::New(info.Env(), _page->GetRotation());
}
Napi::Value
Page::GetNumFields(const CallbackInfo& info)
{
  return Napi::Number::New(info.Env(), _page->GetNumFields());
}

Napi::Value
Page::GetFields(const CallbackInfo& info)
{
  try {
    auto fields = Napi::Array::New(info.Env());
    for (size_t i = 0; i < static_cast<size_t>(_page->GetNumFields()); ++i) {
      auto obj = Napi::Object::New(info.Env());
      auto field = _page->GetField(static_cast<int>(i));
      obj.Set("index", Napi::Number::New(info.Env(), i));
      Page::GetFieldObject(obj, field);
      fields.Set(static_cast<uint32_t>(i), obj);
    }
    return fields;
  } catch (PdfError& err) {
    stringstream errMsg;
    errMsg << "An error occured in PoDoFo: " << err.GetError() << endl;
    throw Napi::Error::New(info.Env(), errMsg.str());
  }
}

void
Page::GetFieldObject(Napi::Object& obj, PoDoFo::PdfField& field)
{
  string name = field.GetFieldName().GetStringUtf8();
  string alternateName = field.GetAlternateName().GetStringUtf8();
  string mappingName = field.GetMappingName().GetStringUtf8();
  bool required = field.IsRequired();
  bool isWritable = field.IsReadOnly();
  obj.Set("name", name);
  obj.Set("alternateName", alternateName);
  obj.Set("mappingName", mappingName);
  obj.Set("required", required);
  obj.Set("readOnly", isWritable);
  switch (field.GetType()) {
    case ePdfField_TextField: {
      PdfTextField textField(field);
      string fieldValue = textField.GetText().GetStringUtf8();
      long maxLen = textField.GetMaxLen();
      bool multiLine = textField.IsMultiLine();
      obj.Set("value", fieldValue);
      obj.Set("maxLength", static_cast<double>(maxLen));
      obj.Set("isMultiLine", multiLine);
      obj.Set("type", "TextField");
      break;
    }
    case ePdfField_CheckBox: {
      PdfCheckBox checkBox(field);
      bool checkBoxValue = checkBox.IsChecked();
      string checkBoxCaption = checkBox.GetCaption().GetStringUtf8();
      obj.Set("value", checkBoxValue);
      obj.Set("caption", checkBoxCaption);
      obj.Set("type", "CheckBox");
      break;
    }
    case ePdfField_ComboBox: {
      PdfComboBox comboBox(field);
      string comboValue =
        comboBox.GetItem(comboBox.GetSelectedItem()).GetStringUtf8();
      obj.Set("type", "ComboBox");
      obj.Set("selected", comboValue);
      break;
    }
    case ePdfField_ListBox: {
      PdfListBox listBox(field);
      string listValue =
        listBox.GetItem(listBox.GetSelectedItem()).GetStringUtf8();
      obj.Set("type", "ListBox");
      obj.Set("value", listValue);
      break;
    }
    case ePdfField_PushButton: {
      PdfPushButton pushButton(field);
      string pushCaption = pushButton.GetCaption().GetStringUtf8();
      obj.Set("type", "PushButton");
      obj.Set("caption", pushCaption);
      break;
    }
    case ePdfField_RadioButton: {
      obj.Set("type", "RadioButton");
      break;
    }
    case ePdfField_Signature: {
      obj.Set("type", "Signature");
      break;
    }
    case ePdfField_Unknown: {
      obj.Set("type", "Unknown");
      break;
    }
  }
}

void
Page::SetRotation(const CallbackInfo& info, const Napi::Value& value)
{
  if (!value.IsNumber()) {
    throw Napi::Error::New(info.Env(), "Rotation must be number");
  }
  int rotate = value.As<Number>();
  _page->SetRotation(rotate);
}

Napi::Value
Page::GetFieldIndex(const CallbackInfo& info)
{
  AssertFunctionArgs(info, 1, { napi_valuetype::napi_string });
  string key = info[0].As<String>().Utf8Value();
  int index = -1;
  for (int i = 0; i < _page->GetNumFields(); ++i) {
    PdfField field = _page->GetField(i);
    string name = field.GetFieldName().GetStringUtf8();
    string alternate = field.GetAlternateName().GetStringUtf8();
    string mapping = field.GetMappingName().GetStringUtf8();
    if (key == name || key == alternate || key == mapping) {
      index = i;
      break;
    }
  }
  return Napi::Number::New(info.Env(), index);
}
