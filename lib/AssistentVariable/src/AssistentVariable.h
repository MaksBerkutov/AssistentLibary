#ifndef AssistentVariable_h
#define AssistentVariable_h
#include <Arduino.h>
#include <Map.h>

class IData
{
public:
  virtual void SetName(String Name) = 0;
  virtual String GetData() = 0;
  virtual ~IData() {}
};

class AssistentDataVariable : public IData
{
  String Name;
  String Value;

public:
  AssistentDataVariable(const String &data) : Value(data)
  {
  }
  void SetName(String Name)
  {
    this->Name = Name;
  }

  String
  GetData() override
  {
    return "\"" + this->Name + "\":\"" + this->Value + "\"";
  }
};
class AssistentDataList : public IData
{
  String Name;

  LinkedList<String> Value;

public:
  AssistentDataList()
  {
  }
  AssistentDataList(String *_Value, int size)
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
class AssistentVariable
{

  Map<String, IData *> Data;

public:
  IData *&operator[](const String &key)
  {
    return Data[key];
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

#endif