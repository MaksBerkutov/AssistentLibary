#include <unity.h>
#include "AssistentEasy.h"

void EasyPingHandler(String arg)
{
  (void)arg;
}

AssistentVariable::Variable EasyStateHandler(String arg)
{
  (void)arg;
  return AssistentVariable::Variable();
}

void test_easy_api_compiles()
{
  AssistentRelayDevice relayDevice(
      "Node",
      "00112233445566778899AABBCCDDEEFF",
      "SSID",
      "PASSWORD");

  relayDevice.addRelay("led", 2);
  relayDevice.on("PING", EasyPingHandler);
  relayDevice.onRequest("GET_STATE", EasyStateHandler);
  relayDevice.set("mode", "auto");

  TEST_ASSERT_TRUE(relayDevice.relay("led").isValid());

  AssistentClimateDevice climateDevice(
      "Climate",
      "00112233445566778899AABBCCDDEEFF",
      "SSID",
      "PASSWORD");

  climateDevice.addDht22("room", 4);

  TEST_ASSERT_TRUE(true);
}

void runTestEasyCompile()
{
  RUN_TEST(test_easy_api_compiles);
}
