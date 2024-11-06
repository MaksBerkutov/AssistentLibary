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
  webServer.send(200, "text/plain", "EEPROM cleared");
}
String AssistenWiFi::StandartHandler(String str, bool &flag)
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
  String response = ThisStandartCommand(str);

  if (response.length() != 0)
    return response;

  for (int i = 0; i < SizeCMD; i++)
    if (CMD[i] == str)
    {
      HandlerCMDS[i]();
      return "Succses";
    }

  for (int i = 0; i < SizeCMDRec; i++)
    if ((CMDRec[i] + "_REC") == str)
      return HandlerCMDSRec[i]().GetStringVal();
  ASSISTENT_debugln("END StandartHandler");

  flag = false;
  return "Not find command [" + str + "]";
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

String AssistenWiFi::ThisStandartCommand(String str)
{
  if (strcmp(str.c_str(), "SERV_GAI") == 0)
  {

    ASSISTENT_debug("DEBUG : ");
    String msg(this->PlatName);

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
  else
  {
    return "";
  }
}

void AssistenWiFi::Begin(String AesKey, String Name, String *CMD, HandlerCMD *HandlerCMDS,
                         int SizeCMD, String *CMDRec, HandlerCMDRec *HandlerCMDSRec, int SizeCMDRec,
                         char *ssid, char *password, int BhaudRate, OnNewMessageFromServer handler)
{
  EEPROM.begin(1000);
  this->myAes.begin(AesKey);
  this->CMD = CMD;
  this->HandlerCMDS = HandlerCMDS;
  this->SizeCMD = SizeCMD;
  this->CMDRec = CMDRec;
  this->HandlerCMDSRec = HandlerCMDSRec;
  this->SizeCMDRec = SizeCMDRec;
#ifdef ASSISTENT_DEBUG
  Serial.begin(BhaudRate);
#endif

  PlatName = Name;
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

void AssistenWiFi::IoTMessage(AssistentVariable data)
{

  String jsonResponse = "{" + myAes.encryptMessage(data.GetStringVal()) + ",\"name\":\"" + PlatName + "\"}";
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

String AssistenWiFi::decryptMessageFromJSON(String jsonMessage)
{
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, jsonMessage);
  const char *encryptedMessage = doc["command"];
  String IV = doc["IV"];
  byte AESIV[16];
  myAes.hexStringToByteArray(IV, AESIV, 16);
  ASSISTENT_debug("Message: ");
  ASSISTENT_debugln(encryptedMessage);
  String decryptedMessage = myAes.decryptMessage(encryptedMessage, AESIV);

  ASSISTENT_debugln("Decrypted message: " + decryptedMessage);
  return decryptedMessage;
}