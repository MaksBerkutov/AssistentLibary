
#include <AssistentLibrary.h>
#define PinLed 2
#define ReleyPin1 D5
#define ReleyPin2 D6

bool State = true;
bool Rele1 = false;
bool Rele2 = false;

String CMDRec[] = {
    "GetStates"};
String ReleToString(bool value)
{
  return value ? "open" : "close";
}
AssistentVariable GetState()
{

  return AssistentVariable(new String[3]{"State", "Rele1", "Rele2"}, new String[3]{ReleToString(State), ReleToString(Rele1), ReleToString(Rele2)}, 3);
}
HandlerCMDRec HCmdRec[] = {
    GetState};

void ON()
{
  State = true;
  digitalWrite(PinLed, LOW);
}
void OFF()
{
  State = false;
  digitalWrite(PinLed, HIGH);
}
void ON_Rele1()
{
  Rele1 = true;
  digitalWrite(ReleyPin1, HIGH);
}
void OFF_Rele1()
{
  Rele1 = false;
  digitalWrite(ReleyPin1, LOW);
}
void ON_Rele2()
{
  Rele2 = true;
  digitalWrite(ReleyPin2, HIGH);
}
void OFF_Rele2()
{
  Rele2 = false;
  digitalWrite(ReleyPin2, LOW);
}

HandlerCMD HCmd[] = {
    ON,
    OFF,
    ON_Rele1,
    OFF_Rele1,
    ON_Rele2,
    OFF_Rele2};

String CMD[]{
    "ON",
    "OFF",
    "ON_Rele1",
    "OFF_Rele1",
    "ON_Rele2",
    "OFF_Rele2"

};

String NameBoard = "Arduino1";
String AesKey = "30555C24496F7F124026365948694E0F";
AssistenWiFi assistent;

void setup()
{
  pinMode(PinLed, OUTPUT);
  pinMode(ReleyPin1, OUTPUT);
  pinMode(ReleyPin2, OUTPUT);
  assistent.Begin(AesKey, NameBoard, CMD, HCmd, 6, CMDRec, HCmdRec, 1, (char *)"DOM", (char *)"07012004");
}

void loop()
{
  assistent.Handle();
}