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

    Types::IData *Get(const String &key)
    {
      return Data.find(key);
    }

    template <typename T>
    static T *ToType(Types::IData *d)
    {
      return (d != nullptr && d->GetType() == T::Type) ? static_cast<T *>(d) : nullptr;
    }

    String GetString()
    {
      String result = "{";
      bool hasData = false;
      if (Data.list.moveToStart())
      {
        do
        {
          if (Data.list.getCurrent().value == nullptr)
            continue;

          hasData = true;
          Data.list.getCurrent().value->SetName(Data.list.getCurrent().key);

          result += Data.list.getCurrent().value->GetData() + ",";
        } while (Data.list.next());
      }

      if (hasData)
      {
        result.remove(result.length() - 1);
      }

      return result + "}";
    }
  };
}
#endif
