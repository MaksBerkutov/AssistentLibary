#ifndef DataVariable_H
#define DataVariable_H
#include <Arduino.h>
#include "IData.h"
namespace AssistentVariable
{
  namespace Types
  {
    class DataVariable : public IData
    {
      String Name;
      String Value;

    public:
      static DataType Type;
      DataType GetType() const override { return VARIABLE; }
      DataVariable(const String &data) : Value(data) {}
      void SetName(String Name) { this->Name = Name; }
      String GetData() override
      {
        return "\"" + this->Name + "\":\"" + this->Value + "\"";
      }
      DataVariable &operator=(const String &newValue)
      {
        this->Value = newValue;
        return *this;
      }
    };
    DataType DataVariable::Type = DataType::VARIABLE;
  }
}
#endif