#ifndef Assistent_h
#define Assistent_h
// CFG
#define ASSISTENT_DEBUG
#define ASSISTENT_OTA

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <AssistentAES.h>
#include <EEPROM.h>
#include <AssistentVariable.h>
#ifdef ASSISTENT_OTA
#include <ESP8266httpUpdate.h>
#include <FS.h>
#endif

typedef void (*OnNewMessageFromServer)(String message);
typedef void (*HandlerCMD)();
typedef AssistentVariable (*HandlerCMDRec)();

class AssistenWiFi
{

  unsigned long lastExecutionTime = 0;
  char *hostname = "";
  String PlatName;
  ESP8266WebServer webServer;
  AssistentAES myAes;
  String *CMD;
  HandlerCMD *HandlerCMDS;
  int SizeCMD;
  String *CMDRec;
  HandlerCMDRec *HandlerCMDSRec;
  int SizeCMDRec;
  OnNewMessageFromServer handler;
  WiFiClient wifiClient;
  bool UsingStandartHandler = true;
  bool ipLoaded = false;

private:
  String StandartHandler(String str, bool &flag);
  void hexStringToByteArray(const String &hexStr, byte *byteArray);
  String decryptMessageFromJSON(String jsonMessage);
  String ThisStandartCommand(String str);
  String SendPostRequest(const char *url, const char *jsonPayload = nullptr);

  void LoadIpFromEprom(String SetIp = "");
#ifdef ASSISTENT_OTA
  void ReadOTA();
#endif
  void clear_eprom();
  void ASSISTENT_debug(String Text);
  void ASSISTENT_debugln(String Text);

public:
  char server_ip[16] = "\0";

  bool ValidateIP();
  AssistenWiFi()
      : webServer(80)
  {
  }
  void Begin(String AesKey, String Name, String *CMD, HandlerCMD *HandlerCMDS, int SizeCMD, String *CMDRec, HandlerCMDRec *HandlerCMDSRec, int SizeCMDRec, char *ssid, char *password, int BhaudRate = 9600, OnNewMessageFromServer handler = NULL);
  void Reader();
  void Handle();
  void IoTMessage(AssistentVariable data);
};
#endif