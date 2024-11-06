
#include <unity.h>

void setUp()
{
}

void tearDown()
{
}
extern void runTestDecrypt();
extern void runTestValidateIP();
void RUN_UNITY_TESTS()
{
  UNITY_BEGIN();
  runTestDecrypt();
  runTestValidateIP();
  UNITY_END();
}
#ifdef ARDUINO

#include <Arduino.h>
void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  delay(2000);
  RUN_UNITY_TESTS();
}

void loop()
{
  digitalWrite(13, HIGH);
  delay(100);
  digitalWrite(13, LOW);
  delay(500);
}

#else

int main(int argc, char **argv)
{
  RUN_UNITY_TESTS();

  return 0;
}

#endif