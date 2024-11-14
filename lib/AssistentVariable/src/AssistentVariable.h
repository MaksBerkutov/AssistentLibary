#ifndef AssistentVariable_h
#define AssistentVariable_h
#include <Arduino.h>
#include <Map.h>

#include <IData.h>
namespace AssistentVariable
{
  class Variable
  {

    Map<String, Types::IData *> Data;

  public:
    Types::IData *&operator[](const String &key)
    {
      return Data[key];
    }

    template <typename T>
    static T *ToType(Types::IData *d)
    {
      return (d->GetType() == T::Type) ? static_cast<T *>(d) : nullptr;
    }

    String GetString()
    {
      String result = "{";
      if (Data.list.moveToStart())
      {
        do
        {

          Data.list.getCurrent().value->SetName(Data.list.getCurrent().key);

          result += Data.list.getCurrent().value->GetData() + ",";
        } while (Data.list.next());
      }
      result.remove(result.length() - 1);
      return result + "}";
    }
  };
}
#endif