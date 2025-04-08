#include "AssistentLibrary.h"
void AssistenWiFi::LoadIpFromEprom(String SetIp)
{
  if (ipLoaded)
    return;
  if (!ValidateIP())
  {
    EEPROM.get(0, server_ip);

    if (!ValidateIP())
    {

      strncpy(server_ip, SetIp.c_str(), sizeof(server_ip) - 1);
      ASSISTENT_debugln(server_ip);

      EEPROM.put(0, server_ip);
      EEPROM.commit();
    }
    else
      ipLoaded = true;
  }
  else
    ipLoaded = true;
}

bool AssistenWiFi::ValidateIP()
{
  int octets = 0;
  int digits = 0;
  int value = 0;

  for (int i = 0; i < 16; i++)
  {
    if (server_ip[i] == '\0')
      break;

    if (server_ip[i] == '.')
    {
      if (digits == 0 || digits > 3 || value > 255)
        return false;
      octets++;
      digits = 0;
      value = 0;
    }
    else if (isdigit(server_ip[i]))
    {
      digits++;
      value = value * 10 + (server_ip[i] - '0');
    }
    else
    {
      return false;
    }
  }

  return (octets == 3 && digits > 0 && digits <= 3 && value <= 255);
}
void AssistenWiFi::clear_eprom()
{
  EEPROM.put(0, "AAA.AAA.AAA.AAA");
  EEPROM.commit();
  this->cfg->Clear();
  webServer.send(200, "text/plain", "EEPROM cleared");
}
String AssistenWiFi::StandartHandler(Package pack, bool &flag)
{
  ASSISTENT_debug("IP EPROM Start");
  ASSISTENT_debugln(server_ip);

  flag = true;

  String ipStr = webServer.client().remoteIP().toString();
  LoadIpFromEprom(ipStr.c_str());
  ASSISTENT_debug("IP Client ");
  ASSISTENT_debugln(ipStr.c_str());
  ASSISTENT_debug("IP EPROM ");
  ASSISTENT_debugln(server_ip);

  if (!(ipStr == String(server_ip)))
  {
    flag = false;
    return "Error rebind plate! Unauthorizted";
  }
  String response = ThisStandartCommand(pack);

  if (response.length() != 0)
    return response;

  for (int i = 0; i < SizeCMD; i++)
    if (CMD[i] == pack.Command)
    {
      HandlerCMDS[i](pack.Arg);
      return "Succses";
    }

  for (int i = 0; i < SizeCMDRec; i++)
    if ((CMDRec[i] + "_REC") == pack.Command)
      return HandlerCMDSRec[i](pack.Arg).GetString();
  ASSISTENT_debugln("END StandartHandler");

  flag = false;
  return "Not find command [" + pack.Command + "]";
}

void AssistenWiFi::hexStringToByteArray(const String &hexStr, byte *byteArray)
{
  for (int i = 0; i < hexStr.length(); i += 2)
  {
    String byteString = hexStr.substring(i, i + 2);
    byteArray[i / 2] = strtol(byteString.c_str(), NULL, 16);
  }
}
#ifdef ASSISTENT_OTA
#ifdef ESP32
void AssistenWiFi::ReadOTA()
{
  String url = webServer.arg("url");
  ASSISTENT_debugln("Получен URL: " + url);

  webServer.send(200, "text/plain", "Update started");

  ASSISTENT_debugln("Начало OTA обновления...");

  HTTPClient http;
  http.begin(url); // Подключаемся к URL
  int httpCode = http.GET();

  if (httpCode == HTTP_CODE_OK)
  {
    int contentLength = http.getSize();
    bool canBegin = Update.begin(contentLength);

    if (canBegin)
    {
      WiFiClient &client = http.getStream();

      size_t written = Update.writeStream(client);
      if (written == contentLength)
      {
        ASSISTENT_debugln("Прошивка успешно загружена!");
      }
      else
      {
        ASSISTENT_debugln("Записано только: " + String(written) + "/" + String(contentLength));
      }

      if (Update.end())
      {
        if (Update.isFinished())
        {
          ASSISTENT_debugln("Обновление успешно, перезагрузка...");
          webServer.send(200, "text/plain", "Update Successful");
          delay(100);
          ESP.restart();
        }
        else
        {
          ASSISTENT_debugln("Ошибка завершения обновления!");
          webServer.send(500, "text/plain", "Update Failed");
        }
      }
      else
      {
        ASSISTENT_debugln("Ошибка обновления. Код ошибки: " + String(Update.getError()));
        webServer.send(500, "text/plain", "Update Failed");
      }
    }
    else
    {
      ASSISTENT_debugln("Недостаточно места для обновления");
      webServer.send(500, "text/plain", "Not enough space for OTA update");
    }
  }
  else
  {
    ASSISTENT_debugln("HTTP ошибка: " + String(httpCode));
    webServer.send(500, "text/plain", "HTTP Update Failed");
  }

  http.end();
}
#else
void AssistenWiFi::ReadOTA()
{
  String url = webServer.arg("url");
  ASSISTENT_debugln("Пришл URL: " + url);
  webServer.send(200, "text/plain", "Update started");
  t_httpUpdate_return ret = ESPhttpUpdate.update(wifiClient, url);
  ;
  ASSISTENT_debugln("Начало OTA обновления...");
  switch (ret)
  {
  case HTTP_UPDATE_FAILED:
    ASSISTENT_debugln("No Updates Available");

    webServer.send(500, "text/plain", "Update Failed");
    break;
  case HTTP_UPDATE_NO_UPDATES:
    ASSISTENT_debugln("No Updates Available");

    webServer.send(200, "text/plain", "No Updates Available");
    break;
  case HTTP_UPDATE_OK:
    ASSISTENT_debugln("Обновление успешно, перезагрузка...");
    webServer.send(200, "text/plain", "Update Successful");
    delay(100);
    ESP.restart();
    break;
  }
}

#endif
#endif
void AssistenWiFi::ASSISTENT_debugln(String Text)
{
#ifdef ASSISTENT_DEBUG

  Serial.println(Text);
#endif
}
void AssistenWiFi::ASSISTENT_debug(String Text)
{
#ifdef ASSISTENT_DEBUG
  Serial.print(Text);
#endif
}

String AssistenWiFi::ThisStandartCommand(Package pack)
{
  if (strcmp(pack.Command.c_str(), "SERV_GAI") == 0)
  {

    ASSISTENT_debug("DEBUG : ");
    String msg(this->PlatName);
    msg += OTA ? ".1." : ".0.";
    msg += this->CONFIGURE ? "1" : "0";

    // Command non send message
    for (int i = 0; i < SizeCMD; i++)
    {
      ASSISTENT_debugln(CMD[i]);
      msg += "." + CMD[i];
    }

    // Command send result
    for (int i = 0; i < SizeCMDRec; i++)
    {
      ASSISTENT_debugln(CMDRec[i]);
      msg += "." + CMDRec[i] + "_REC";
    }

    return msg;
  }
  else if (strcmp(pack.Command.c_str(), "SERV_GCFG") == 0)
  {
    ASSISTENT_debugln("Состояние конфигурации");
    return this->cfg->GetJson();
  }
  else if (strcmp(pack.Command.c_str(), "SERV_SCFG") == 0)
  {
    this->cfg->Inject(pack.Arg);

    return "Succesful";
  }
  else
  {
    return "";
  }
}

void AssistenWiFi::Begin(String AesKey, String Name, String *CMD, HandlerCMD *HandlerCMDS,
                         int SizeCMD, String *CMDRec, HandlerCMDRec *HandlerCMDSRec, int SizeCMDRec,
                         const char *ssid, const char *password, Config *cfg, int BhaudRate, OnNewMessageFromServer handler)
{
#ifdef ASSISTENT_DEBUG
  Serial.begin(BhaudRate);
#endif
  EEPROM.begin(1000);
  if (cfg == nullptr)
    this->cfg = new Config(new BaseConfig(), "");
  else
  {
#ifdef ESP32
    if (!LittleFS.begin(1024 * 1024))
#else
    if (!LittleFS.begin())

#endif
    {
      Serial.println("Ошибка монтирования LittleFS");
      return;
    }
    else
    {
      Serial.println("Файловая система успешно смонтирована");
    }
    this->cfg = cfg;
    this->cfg->Load();
    this->CONFIGURE = true;
  }

  this->myAes.begin(AesKey);
  this->CMD = CMD;
  this->HandlerCMDS = HandlerCMDS;
  this->SizeCMD = SizeCMD;
  this->CMDRec = CMDRec;
  this->HandlerCMDSRec = HandlerCMDSRec;
  this->SizeCMDRec = SizeCMDRec;

  PlatName = Name;
  WiFi.setHostname(PlatName.c_str());
  WiFi.begin(ssid, password);
  ASSISTENT_debug("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    ASSISTENT_debug(" .");
  }
  ASSISTENT_debug("\nConnected to WiFi.\n SSID:");

  ASSISTENT_debugln(ssid);
  ASSISTENT_debug("IP:");
  ASSISTENT_debugln(WiFi.localIP().toString());

  // SetupHandler
  if (handler == NULL)
  {
    this->handler = handler;
    UsingStandartHandler = false;
  }
  webServer.on("/command", HTTP_POST, [this]()
               { this->Reader(); });
  webServer.on("/clear", HTTP_POST, [this]()
               { this->clear_eprom(); });
#ifdef ASSISTENT_OTA
  webServer.on("/ota", HTTP_POST, [this]()
               { this->ReadOTA(); });
#endif
  webServer.begin();
}

void AssistenWiFi::IoTMessage(AssistentVariable::Variable data)
{

  String jsonResponse = "{" + myAes.encryptMessage(data.GetString()) + ",\"name\":\"" + PlatName + "\"}";
  LoadIpFromEprom();
  if (ValidateIP())
    SendPostRequest(("http://" + String(server_ip) + "/iot/receive").c_str(), jsonResponse.c_str());
}

String AssistenWiFi::SendPostRequest(const char *url, const char *jsonPayload)
{
  if (WiFi.status() == WL_CONNECTED)
  {
    HTTPClient http;
    ASSISTENT_debugln(url);
    if (http.begin(wifiClient, url))
    {
      http.addHeader("Content-Type", "application/json");

      int httpCode;
      if (jsonPayload)
      {
        httpCode = http.POST(jsonPayload);
      }
      else
      {
        httpCode = http.POST("");
      }
      if (httpCode > 0)
      {
        String response = http.getString();
        ASSISTENT_debugln("Response from server: " + response);
        http.end();
        return response;
      }
      else
      {
        Serial.printf("Error on POST request, HTTP code: %d\n", httpCode);
        http.end();
        return "";
      }
    }
    else
    {
      ASSISTENT_debugln("Unable to connect to the URL");
      return "";
    }
  }
  else
  {
    ASSISTENT_debugln("WiFi not connected");
    return "";
  }
}
void AssistenWiFi::Reader()
{
  if (webServer.hasArg("plain"))
  {

    String requestBody = webServer.arg("plain");
    bool responce_status = true;
    String responce = StandartHandler(decryptMessageFromJSON(requestBody), responce_status);

    if (!responce_status)
    {
      webServer.send(400, "application/json", "{\"error\":\"" + responce + "\"}");
      return;
    }
    String msg = myAes.encryptMessage(responce);
    String jsonResponse = "{" + msg + "}";
    webServer.send(200, "application/json", jsonResponse);
  }
  else
  {
    webServer.send(400, "application/json", "{\"error\":\"No data received\"}");
  }
}
void AssistenWiFi::Handle()
{
  webServer.handleClient();
}

Package AssistenWiFi::decryptMessageFromJSON(String jsonMessage)
{
  Package pack;
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, jsonMessage);
  const char *encryptedMessage = doc["command"];
  const char *encryptedArg = doc["arg"];
  String IV = doc["IV"];
  byte AESIV[16];
  myAes.hexStringToByteArray(IV, AESIV, 16);
  ASSISTENT_debug("Command: ");
  ASSISTENT_debugln(encryptedMessage);
  ASSISTENT_debug("Argument: ");
  ASSISTENT_debugln(encryptedArg);
  pack.Arg = myAes.decryptMessage(encryptedArg, AESIV);
  myAes.hexStringToByteArray(IV, AESIV, 16);

  pack.Command = myAes.decryptMessage(encryptedMessage, AESIV);

  ASSISTENT_debugln("Decrypted Command: " + pack.Command);
  ASSISTENT_debugln("Decrypted Argument: " + pack.Arg);
  return pack;
}