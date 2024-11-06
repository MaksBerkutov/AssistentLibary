#ifndef AssistentVariable_h
#define AssistentVariable_h
#include <Arduino.h>
class AssistentVariable
{

  String obj;

public:
  AssistentVariable(String *NameVar, String *Date, int size)
  {
    obj = "{";

    for (int i = 0; i < size; i++)
    {
      if (i == 0)
        obj += "\"" + NameVar[i] + "\":\"" + Date[i] + "\"";
      else
        obj += ",\"" + NameVar[i] + "\":\"" + Date[i] + "\"";
    }
    obj += "}";
  }

  String GetStringVal()
  {
    return obj;
  }
};
#endif