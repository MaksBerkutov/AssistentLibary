#ifndef ASSISTENT_EASY_H
#define ASSISTENT_EASY_H

#include <AssistentLibrary.h>
#include <Types/DataRele.h>
#include <Types/DataTemperature.h>
#include <Types/DataVariable.h>

typedef void (*EasyHandler)(String Arg);
typedef AssistentVariable::Variable (*EasyRequestHandler)(String Arg);

class AssistentEasyDevice
{
public:
  static const int MaxCommands = 12;
  static const int MaxRequests = 12;
  static const int MaxRelays = 12;

  class RelayRef
  {
  public:
    RelayRef(AssistentVariable::Types::DataRele *relay = NULL);
    bool isValid() const;
    void on();
    void off();
    void toggle();

  private:
    AssistentVariable::Types::DataRele *relay;
  };

  AssistentEasyDevice(const String &deviceName, const String &aesKey, const char *ssid, const char *password, int baudRate = 9600);

  bool on(const String &command, EasyHandler handler);
  bool onRequest(const String &command, EasyRequestHandler handler);

  AssistentEasyDevice &set(const String &key, const String &value);
  AssistentEasyDevice &addRelay(const String &name, byte pin);
  AssistentEasyDevice &addDht22(const String &name, byte pin);

  RelayRef relay(const String &name);
  AssistentVariable::Variable &values();

  void begin();
  void run();
  void sendState();

  static void DispatchCommandSlot(int index, String arg);
  static AssistentVariable::Variable DispatchRequestSlot(int index, String arg);

protected:
  static void BuiltinRelayOn(String arg);
  static void BuiltinRelayOff(String arg);
  static void BuiltinRelayToggle(String arg);
  static AssistentVariable::Variable BuiltinState(String arg);

  void dispatchCommand(int index, String arg);
  AssistentVariable::Variable dispatchRequest(int index, String arg);

  void rememberRelay(const String &name);
  AssistentVariable::Types::DataRele *findRelay(const String &name);
  void setRelayState(const String &name, bool enabled);
  void toggleRelay(const String &name);

  AssistenWiFi device;
  AssistentVariable::Variable variable;
  String deviceName;
  String aesKey;
  const char *ssid;
  const char *password;
  int baudRate;

  String commandNames[MaxCommands];
  String requestNames[MaxRequests];
  EasyHandler commandHandlers[MaxCommands];
  EasyRequestHandler requestHandlers[MaxRequests];

  String relayNames[MaxRelays];
  int commandCount;
  int requestCount;
  int relayCount;

private:
  // The easy facade uses one active dispatcher, which matches the usual
  // Arduino sketch pattern: one device per sketch.
  static AssistentEasyDevice *activeInstance;
};

class AssistentRelayDevice : public AssistentEasyDevice
{
public:
  AssistentRelayDevice(const String &deviceName, const String &aesKey, const char *ssid, const char *password, int baudRate = 9600);
  AssistentRelayDevice &addRelay(const String &name, byte pin);
};

class AssistentClimateDevice : public AssistentEasyDevice
{
public:
  AssistentClimateDevice(const String &deviceName, const String &aesKey, const char *ssid, const char *password, int baudRate = 9600);
  AssistentClimateDevice &addDht22(const String &name, byte pin);
};

#endif
