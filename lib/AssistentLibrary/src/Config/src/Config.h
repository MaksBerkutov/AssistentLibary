// #ifndef AssistentConfig_h
// #define AssistentConfig_h
// #include <FS.h>
// #include <LittleFS.h>
//
// class BaseConfig
//{
//
// public:
//   virtual void Serialize(File &file) {
//
//   };
//
//   virtual void Deserialize(File &file) {
//
//   };
// };
//
// class Config
//{
// private:
//   BaseConfig *target;
//   const char *NameFile;
//
// public:
//   static void Setup()
//   {
//     if (!LittleFS.begin())
//     {
//       Serial.println("Ошибка монтирования LittleFS");
//       return;
//     }
//   }
//   Config(BaseConfig *target, const char *NameFile)
//   {
//     this->target = target;
//     this->NameFile = NameFile;
//     this->Load();
//   }
//   void Save()
//   {
//     File configFile = LittleFS.open(NameFile, "w");
//     if (!configFile)
//     {
//       Serial.println("Ошибка при открытии файла для записи");
//       return;
//     }
//     target->Serialize(configFile);
//     configFile.close();
//   }
//   void Load()
//   {
//     File configFile = LittleFS.open(NameFile, "r");
//     if (!configFile)
//     {
//       Serial.println("Ошибка при открытии файла для записи");
//       return;
//     }
//     target->Deserialize(configFile);
//     configFile.close();
//   }
// };
//
// #endif

#ifndef AssistentConfig_h
#define AssistentConfig_h
#include <ArduinoJson.h>
#include <LittleFS.h>

class BaseConfig
{
public:
  virtual void Serialize(DynamicJsonDocument &doc) {}
  virtual void Deserialize(DynamicJsonDocument &doc) {}
};

class Config
{
private:
  BaseConfig *target;
  const char *NameFile;

public:
  Config(BaseConfig *target, const char *NameFile)
  {
    this->target = target;
    this->NameFile = NameFile;
    this->Load();
  }

  void Save()
  {
    File configFile = LittleFS.open(NameFile, "w");
    if (!configFile)
    {
      Serial.println("Ошибка при открытии файла для записи");
      return;
    }

    DynamicJsonDocument doc(1024);
    target->Serialize(doc);

    serializeJson(doc, configFile);
    configFile.close();
  }

  void Load()
  {
    File configFile = LittleFS.open(NameFile, "r");
    if (!configFile)
    {
      Serial.println("Ошибка при открытии файла для чтения!");
      return;
    }

    DynamicJsonDocument doc(1024);

    DeserializationError error = deserializeJson(doc, configFile);
    if (error)
    {
      Serial.println("Ошибка при десериализации JSON");
      configFile.close();
      return;
    }

    target->Deserialize(doc);
    configFile.close();
  }
  void Inject(const String &jsonString)
  {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, jsonString);

    if (error)
    {
      Serial.print(F("Ошибка десериализации: "));
      Serial.println(error.f_str());
      return;
    }

    File configFile = LittleFS.open(NameFile, "w");
    if (!configFile)
    {
      Serial.println("Ошибка при открытии файла для записи");
      return;
    }
    target->Deserialize(doc);
    serializeJson(doc, configFile);
    configFile.close();
  }
  String GetJson()
  {
    DynamicJsonDocument doc(1024);
    target->Serialize(doc);
    String jsonString;

    serializeJson(doc, jsonString);
    return jsonString;
  }
  void Clear()
  {
    if (LittleFS.exists(NameFile))
    {
      if (LittleFS.remove(NameFile))
      {
        Serial.println("Файл успешно удален.");
      }
      else
      {
        Serial.println("Ошибка при удалении файла.");
      }
    }
    else
    {
      Serial.println("Файл не существует.");
    }
  }
};

#endif