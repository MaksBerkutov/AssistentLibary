#include <AssistentLibrary.h>
#include <Types/DataRele.h>
#define PinLed 2
#define ReleyPin1 15
#define ReleyPin2 16

String NameBoard = "Board_Name";
String AesKey = "Aes_Key";
const char *SSID = "SSID";
const char *PASSWORD = "PASSWORD";
AssistenWiFi assistent;
AssistentVariable::Variable variable;
String CMDRec[] = {
    "GetStates",
};
AssistentVariable::Variable GetState(String Arg)
{
  return variable;
}
HandlerCMDRec HCmdRec[] = {
    GetState,
};

void ON(String Arg)
{
  if (Arg.isEmpty())
    return;
  auto rele = AssistentVariable::Variable::ToType<AssistentVariable::Types::DataRele>(variable[Arg]);
  rele->ON();
}
void OFF(String Arg)
{
  if (Arg.isEmpty())
    return;
  auto rele = AssistentVariable::Variable::ToType<AssistentVariable::Types::DataRele>(variable[Arg]);
  rele->OFF();
}

HandlerCMD HCmd[] = {
    ON,
    OFF,
};
String CMD[]{
    "ON",
    "OFF",
};

void setup()
{

  assistent.Begin(AesKey, NameBoard, CMD, HCmd, 6, CMDRec, HCmdRec, 1, SSID, PASSWORD);
  variable["led"] = new AssistentVariable::Types::DataRele(PinLed);
  variable["Rele1"] = new AssistentVariable::Types::DataRele(ReleyPin1);
  variable["Rele2"] = new AssistentVariable::Types::DataRele(ReleyPin2);

  Serial.println(variable.GetString());
}

void loop()
{
  assistent.Handle();
}
