#include "AssistentEasy.h"

AssistentEasyDevice *AssistentEasyDevice::activeInstance = NULL;

namespace
{
  void EasyCommandBridge0(String arg) { AssistentEasyDevice::DispatchCommandSlot(0, arg); }
  void EasyCommandBridge1(String arg) { AssistentEasyDevice::DispatchCommandSlot(1, arg); }
  void EasyCommandBridge2(String arg) { AssistentEasyDevice::DispatchCommandSlot(2, arg); }
  void EasyCommandBridge3(String arg) { AssistentEasyDevice::DispatchCommandSlot(3, arg); }
  void EasyCommandBridge4(String arg) { AssistentEasyDevice::DispatchCommandSlot(4, arg); }
  void EasyCommandBridge5(String arg) { AssistentEasyDevice::DispatchCommandSlot(5, arg); }
  void EasyCommandBridge6(String arg) { AssistentEasyDevice::DispatchCommandSlot(6, arg); }
  void EasyCommandBridge7(String arg) { AssistentEasyDevice::DispatchCommandSlot(7, arg); }
  void EasyCommandBridge8(String arg) { AssistentEasyDevice::DispatchCommandSlot(8, arg); }
  void EasyCommandBridge9(String arg) { AssistentEasyDevice::DispatchCommandSlot(9, arg); }
  void EasyCommandBridge10(String arg) { AssistentEasyDevice::DispatchCommandSlot(10, arg); }
  void EasyCommandBridge11(String arg) { AssistentEasyDevice::DispatchCommandSlot(11, arg); }

  AssistentVariable::Variable EasyRequestBridge0(String arg) { return AssistentEasyDevice::DispatchRequestSlot(0, arg); }
  AssistentVariable::Variable EasyRequestBridge1(String arg) { return AssistentEasyDevice::DispatchRequestSlot(1, arg); }
  AssistentVariable::Variable EasyRequestBridge2(String arg) { return AssistentEasyDevice::DispatchRequestSlot(2, arg); }
  AssistentVariable::Variable EasyRequestBridge3(String arg) { return AssistentEasyDevice::DispatchRequestSlot(3, arg); }
  AssistentVariable::Variable EasyRequestBridge4(String arg) { return AssistentEasyDevice::DispatchRequestSlot(4, arg); }
  AssistentVariable::Variable EasyRequestBridge5(String arg) { return AssistentEasyDevice::DispatchRequestSlot(5, arg); }
  AssistentVariable::Variable EasyRequestBridge6(String arg) { return AssistentEasyDevice::DispatchRequestSlot(6, arg); }
  AssistentVariable::Variable EasyRequestBridge7(String arg) { return AssistentEasyDevice::DispatchRequestSlot(7, arg); }
  AssistentVariable::Variable EasyRequestBridge8(String arg) { return AssistentEasyDevice::DispatchRequestSlot(8, arg); }
  AssistentVariable::Variable EasyRequestBridge9(String arg) { return AssistentEasyDevice::DispatchRequestSlot(9, arg); }
  AssistentVariable::Variable EasyRequestBridge10(String arg) { return AssistentEasyDevice::DispatchRequestSlot(10, arg); }
  AssistentVariable::Variable EasyRequestBridge11(String arg) { return AssistentEasyDevice::DispatchRequestSlot(11, arg); }

  HandlerCMD EasyCommandBridgeTable[AssistentEasyDevice::MaxCommands] = {
      EasyCommandBridge0,
      EasyCommandBridge1,
      EasyCommandBridge2,
      EasyCommandBridge3,
      EasyCommandBridge4,
      EasyCommandBridge5,
      EasyCommandBridge6,
      EasyCommandBridge7,
      EasyCommandBridge8,
      EasyCommandBridge9,
      EasyCommandBridge10,
      EasyCommandBridge11};

  HandlerCMDRec EasyRequestBridgeTable[AssistentEasyDevice::MaxRequests] = {
      EasyRequestBridge0,
      EasyRequestBridge1,
      EasyRequestBridge2,
      EasyRequestBridge3,
      EasyRequestBridge4,
      EasyRequestBridge5,
      EasyRequestBridge6,
      EasyRequestBridge7,
      EasyRequestBridge8,
      EasyRequestBridge9,
      EasyRequestBridge10,
      EasyRequestBridge11};
}

AssistentEasyDevice::RelayRef::RelayRef(AssistentVariable::Types::DataRele *relay)
    : relay(relay)
{
}

bool AssistentEasyDevice::RelayRef::isValid() const
{
  return relay != NULL;
}

void AssistentEasyDevice::RelayRef::on()
{
  if (relay != NULL)
    relay->ON();
}

void AssistentEasyDevice::RelayRef::off()
{
  if (relay != NULL)
    relay->OFF();
}

void AssistentEasyDevice::RelayRef::toggle()
{
  if (relay == NULL)
    return;

  if (relay->IsOn())
    relay->OFF();
  else
    relay->ON();
}

AssistentEasyDevice::AssistentEasyDevice(const String &deviceName, const String &aesKey, const char *ssid, const char *password, int baudRate)
    : deviceName(deviceName), aesKey(aesKey), ssid(ssid), password(password), baudRate(baudRate), commandCount(0), requestCount(0), relayCount(0)
{
  for (int i = 0; i < MaxCommands; i++)
    commandHandlers[i] = NULL;

  for (int i = 0; i < MaxRequests; i++)
    requestHandlers[i] = NULL;
}

bool AssistentEasyDevice::on(const String &command, EasyHandler handler)
{
  if (handler == NULL)
    return false;

  for (int i = 0; i < commandCount; i++)
  {
    if (commandNames[i] == command)
    {
      commandHandlers[i] = handler;
      return true;
    }
  }

  if (commandCount >= MaxCommands)
    return false;

  commandNames[commandCount] = command;
  commandHandlers[commandCount] = handler;
  commandCount++;
  return true;
}

bool AssistentEasyDevice::onRequest(const String &command, EasyRequestHandler handler)
{
  if (handler == NULL)
    return false;

  for (int i = 0; i < requestCount; i++)
  {
    if (requestNames[i] == command)
    {
      requestHandlers[i] = handler;
      return true;
    }
  }

  if (requestCount >= MaxRequests)
    return false;

  requestNames[requestCount] = command;
  requestHandlers[requestCount] = handler;
  requestCount++;
  return true;
}

AssistentEasyDevice &AssistentEasyDevice::set(const String &key, const String &value)
{
  AssistentVariable::Types::IData *existing = variable.Get(key);
  AssistentVariable::Types::DataVariable *textValue = AssistentVariable::Variable::ToType<AssistentVariable::Types::DataVariable>(existing);

  if (textValue != NULL)
  {
    *textValue = value;
    return *this;
  }

  if (existing != NULL)
    delete existing;

  variable[key] = new AssistentVariable::Types::DataVariable(value);
  return *this;
}

AssistentEasyDevice &AssistentEasyDevice::addRelay(const String &name, byte pin)
{
  AssistentVariable::Types::IData *existing = variable.Get(name);
  if (existing != NULL)
    delete existing;

  variable[name] = new AssistentVariable::Types::DataRele(pin);
  rememberRelay(name);
  return *this;
}

AssistentEasyDevice &AssistentEasyDevice::addDht22(const String &name, byte pin)
{
  AssistentVariable::Types::IData *existing = variable.Get(name);
  if (existing != NULL)
    delete existing;

  variable[name] = new AssistentVariable::Types::DataTemperature(pin);
  return *this;
}

AssistentEasyDevice::RelayRef AssistentEasyDevice::relay(const String &name)
{
  return RelayRef(findRelay(name));
}

AssistentVariable::Variable &AssistentEasyDevice::values()
{
  return variable;
}

void AssistentEasyDevice::begin()
{
  activeInstance = this;
  device.Begin(aesKey, deviceName, commandNames, EasyCommandBridgeTable, commandCount, requestNames, EasyRequestBridgeTable, requestCount, ssid, password, NULL, baudRate);
}

void AssistentEasyDevice::run()
{
  device.Handle();
}

void AssistentEasyDevice::sendState()
{
  device.IoTMessage(variable);
}

void AssistentEasyDevice::DispatchCommandSlot(int index, String arg)
{
  if (activeInstance != NULL)
    activeInstance->dispatchCommand(index, arg);
}

AssistentVariable::Variable AssistentEasyDevice::DispatchRequestSlot(int index, String arg)
{
  if (activeInstance != NULL)
    return activeInstance->dispatchRequest(index, arg);

  return AssistentVariable::Variable();
}

void AssistentEasyDevice::BuiltinRelayOn(String arg)
{
  if (activeInstance != NULL)
    activeInstance->setRelayState(arg, true);
}

void AssistentEasyDevice::BuiltinRelayOff(String arg)
{
  if (activeInstance != NULL)
    activeInstance->setRelayState(arg, false);
}

void AssistentEasyDevice::BuiltinRelayToggle(String arg)
{
  if (activeInstance != NULL)
    activeInstance->toggleRelay(arg);
}

AssistentVariable::Variable AssistentEasyDevice::BuiltinState(String arg)
{
  (void)arg;
  if (activeInstance != NULL)
    return activeInstance->values();

  return AssistentVariable::Variable();
}

void AssistentEasyDevice::dispatchCommand(int index, String arg)
{
  if (index < 0 || index >= commandCount)
    return;

  if (commandHandlers[index] != NULL)
    commandHandlers[index](arg);
}

AssistentVariable::Variable AssistentEasyDevice::dispatchRequest(int index, String arg)
{
  if (index < 0 || index >= requestCount)
    return AssistentVariable::Variable();

  if (requestHandlers[index] != NULL)
    return requestHandlers[index](arg);

  return AssistentVariable::Variable();
}

void AssistentEasyDevice::rememberRelay(const String &name)
{
  for (int i = 0; i < relayCount; i++)
  {
    if (relayNames[i] == name)
      return;
  }

  if (relayCount < MaxRelays)
  {
    relayNames[relayCount] = name;
    relayCount++;
  }
}

AssistentVariable::Types::DataRele *AssistentEasyDevice::findRelay(const String &name)
{
  return AssistentVariable::Variable::ToType<AssistentVariable::Types::DataRele>(variable.Get(name));
}

void AssistentEasyDevice::setRelayState(const String &name, bool enabled)
{
  AssistentVariable::Types::DataRele *rele = findRelay(name);
  if (rele == NULL)
    return;

  if (enabled)
    rele->ON();
  else
    rele->OFF();
}

void AssistentEasyDevice::toggleRelay(const String &name)
{
  AssistentVariable::Types::DataRele *rele = findRelay(name);
  if (rele == NULL)
    return;

  if (rele->IsOn())
    rele->OFF();
  else
    rele->ON();
}

AssistentRelayDevice::AssistentRelayDevice(const String &deviceName, const String &aesKey, const char *ssid, const char *password, int baudRate)
    : AssistentEasyDevice(deviceName, aesKey, ssid, password, baudRate)
{
  on("ON", BuiltinRelayOn);
  on("OFF", BuiltinRelayOff);
  on("TOGGLE", BuiltinRelayToggle);
  onRequest("STATE", BuiltinState);
}

AssistentRelayDevice &AssistentRelayDevice::addRelay(const String &name, byte pin)
{
  AssistentEasyDevice::addRelay(name, pin);
  return *this;
}

AssistentClimateDevice::AssistentClimateDevice(const String &deviceName, const String &aesKey, const char *ssid, const char *password, int baudRate)
    : AssistentEasyDevice(deviceName, aesKey, ssid, password, baudRate)
{
  onRequest("STATE", BuiltinState);
}

AssistentClimateDevice &AssistentClimateDevice::addDht22(const String &name, byte pin)
{
  AssistentEasyDevice::addDht22(name, pin);
  return *this;
}
