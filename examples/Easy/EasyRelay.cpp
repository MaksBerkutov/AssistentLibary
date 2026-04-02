#include <AssistentEasy.h>

AssistentRelayDevice device(
    "RoomLight",
    "00112233445566778899AABBCCDDEEFF",
    "SSID",
    "PASSWORD");

void setup()
{
  device.addRelay("led", 2);
  device.addRelay("pump", 5);
  device.begin();
}

void loop()
{
  device.run();
}
