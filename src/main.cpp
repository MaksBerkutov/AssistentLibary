#include <AssistentVariable.h>
AssistentVariable variable;
void setup()
{
  Serial.begin(9600);
  variable["device"] = new AssistentDataVariable("true");
  variable["device1"] = new AssistentDataVariable("false");
  variable["device2"] = new AssistentDataList(new String[2]{"MAC1", "MAC2"}, 2);
  Serial.println(variable.GetString());
}

void loop()
{
}
