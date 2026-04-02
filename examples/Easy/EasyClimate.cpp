#include <AssistentEasy.h>

AssistentClimateDevice device(
    "ClimateNode",
    "00112233445566778899AABBCCDDEEFF",
    "SSID",
    "PASSWORD");

void setup()
{
  device.addDht22("room", 4);
  device.begin();
}

void loop()
{
  device.run();
}
