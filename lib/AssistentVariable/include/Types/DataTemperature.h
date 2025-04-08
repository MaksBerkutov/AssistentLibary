#ifndef DataTemperature_H
#define DataTemperature_H
#include <Arduino.h>
#include <DHT.h>
#include "IData.h"

namespace AssistentVariable
{
  namespace Types
  {
    class DataTemperature : public IData
    {
      String Name;
      DHT dht;

    public:
      static DataType Type;
      DataType GetType() const override { return TEMPERATURE; }
      DataTemperature(byte pin) : dht(pin, DHT22)
      {
        dht.begin();
        // dht.setup(pin, DHTesp::DHT22);
      }
      float GetTemperature()
      {
        return dht.readTemperature();
      }
      float GetHumidity()
      {
        return dht.readHumidity();
      }

      void SetName(String Name)
      {
        this->Name = Name;
      }

      String GetData() override
      {
        return "\"" + this->Name + "\":{\"Temperature\":\"" + this->GetTemperature() + "\",\"Humidity\":\"" + this->GetHumidity() + "\"}";
      }
    };
    DataType DataTemperature::Type = DataType::TEMPERATURE;
  }
}
#endif