#include "AssistentLibrary.h"
#include <unity.h>
AssistenWiFi assistentWiFi;

void test_ValidateIP_valid_ip()
{
  AssistenWiFi assistentWiFi;
  strcpy(assistentWiFi.server_ip, "192.168.1.1");
  TEST_ASSERT_TRUE(assistentWiFi.ValidateIP());
}
void test_ValidateIP_longer_than_15_characters()
{
  AssistenWiFi assistentWiFi;
  strcpy(assistentWiFi.server_ip, "192.168.100.1000");
  TEST_ASSERT_FALSE(assistentWiFi.ValidateIP());
}
void test_ValidateIP_max_valid_ip()
{
  AssistenWiFi assistentWiFi;
  strcpy(assistentWiFi.server_ip, "255.255.255.255");
  TEST_ASSERT_TRUE(assistentWiFi.ValidateIP());
}
void test_ValidateIP_ending_with_dot()
{
  AssistenWiFi assistentWiFi;
  strcpy(assistentWiFi.server_ip, "192.168.1.");
  TEST_ASSERT_FALSE(assistentWiFi.ValidateIP());
}
void test_ValidateIP_empty_string()
{
  AssistenWiFi assistentWiFi;
  strcpy(assistentWiFi.server_ip, "");
  TEST_ASSERT_FALSE(assistentWiFi.ValidateIP());
}
void test_ValidateIP_segment_with_more_than_three_digits()
{
  AssistenWiFi assistentWiFi;
  strcpy(assistentWiFi.server_ip, "192.168.1.1000");
  TEST_ASSERT_FALSE(assistentWiFi.ValidateIP());
}
void test_ValidateIP_non_digit_characters()
{
  AssistenWiFi assistentWiFi;
  strcpy(assistentWiFi.server_ip, "192.168.1.a");
  TEST_ASSERT_FALSE(assistentWiFi.ValidateIP());
}
void test_ValidateIP_starting_with_dot()
{
  AssistenWiFi assistentWiFi;
  strcpy(assistentWiFi.server_ip, ".192.168.1.1");
  TEST_ASSERT_FALSE(assistentWiFi.ValidateIP());
}
void test_ValidateIP_more_than_three_dots()
{
  AssistenWiFi assistentWiFi;
  strcpy(assistentWiFi.server_ip, "192.168.1.1.5");
  TEST_ASSERT_FALSE(assistentWiFi.ValidateIP());
}
void test_ValidateIP_less_than_three_dots()
{
  AssistenWiFi assistentWiFi;
  strcpy(assistentWiFi.server_ip, "192.168.1");
  TEST_ASSERT_FALSE(assistentWiFi.ValidateIP());
}

void runTestValidateIP()
{
  RUN_TEST(test_ValidateIP_valid_ip);
  RUN_TEST(test_ValidateIP_longer_than_15_characters);
  RUN_TEST(test_ValidateIP_max_valid_ip);
  RUN_TEST(test_ValidateIP_ending_with_dot);
  RUN_TEST(test_ValidateIP_empty_string);
  RUN_TEST(test_ValidateIP_segment_with_more_than_three_digits);
  RUN_TEST(test_ValidateIP_non_digit_characters);
  RUN_TEST(test_ValidateIP_starting_with_dot);
  RUN_TEST(test_ValidateIP_more_than_three_dots);
  RUN_TEST(test_ValidateIP_starting_with_dot);
  RUN_TEST(test_ValidateIP_less_than_three_dots);
}