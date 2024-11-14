#ifndef DataList_H
#define DataList_H
#include <Arduino.h>
#include "IData.h"
#include <LinkedList.h>

namespace AssistentVariable
{
  namespace Types
  {
    class DataList : public IData
    {
      String Name;

      LinkedList<String> Value;

    public:
      static DataType Type;
      DataType GetType() const override { return LIST; }
      DataList()
      {
      }
      DataList(String *_Value, int size)
      {
        for (int i = 0; i < size; i++)
          this->Value.Append(_Value[i]);
      }
      void SetName(String Name)
      {
        this->Name = Name;
      }
      void add(String Value)
      {
        this->Value.Append(Value);
      }

      String GetData() override
      {
        String Result = "\"" + this->Name + "\":[";

        if (Value.moveToStart())
        {
          do
          {
            Result += "\"" + Value.getCurrent() + "\",";
          } while (Value.next());
          Result.remove(Result.length() - 1);
        }

        return Result + "]";
      }
    };
    DataType DataList::Type = DataType::LIST;
  }
}
#endif