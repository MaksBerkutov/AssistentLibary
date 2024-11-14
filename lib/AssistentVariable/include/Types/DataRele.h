#ifndef DataRele_H
#define DataRele_H
#include <Arduino.h>
#include "IData.h"

namespace AssistentVariable
{
  namespace Types
  {
    class DataRele : public IData
    {
      String Name;
      bool Value;
      byte pin;

    public:
      static DataType Type;
      DataType GetType() const override { return RELE; }
      DataRele(byte pin)
      {
        this->pin = pin;
        this->Value = false;
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
      }
      void ON()
      {
        if (!this->Value)
        {
          this->Value = true;
          digitalWrite(pin, HIGH);
        }
      }
      void OFF()
      {
        if (this->Value)
        {
          this->Value = false;
          digitalWrite(pin, LOW);
        }
      }

      void SetName(String Name)
      {
        this->Name = Name;
      }

      String GetData() override
      {
        return "\"" + this->Name + "\":\"" + (this->Value ? "true" : "false") + "\"";
      }
    };
    DataType DataRele::Type = DataType::RELE;
  }
}
#endif