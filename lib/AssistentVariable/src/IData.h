#ifndef IData_H
#define IData_H
#include <Arduino.h>
namespace AssistentVariable
{
  namespace Types
  {
    enum DataType
    {
      VARIABLE,
      LIST,
      RELE
    };
    class IData
    {
    public:
      static DataType Type;
      virtual DataType GetType() const = 0;
      virtual void SetName(String Name) = 0;
      virtual String GetData() = 0;
      virtual ~IData() {}
    };
  }
}

#endif