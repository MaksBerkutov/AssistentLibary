#include <unity.h>
#include "AssistentAES.h"

void test_decryptMessage_valid_message()
{
  AssistentAES aes;
  const char *encryptedMessage = "SGVsbG8gV29ybGQh"; // Base64 encoded "Hello World!"
  byte iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

  String decryptedMessage = aes.decryptMessage(encryptedMessage, iv);

  TEST_ASSERT_EQUAL_STRING("Hello World!", decryptedMessage.c_str());
}

void test_decryptMessage_empty_input()
{
  AssistentAES aes;
  const char *emptyInput = "";
  byte iv[16] = {0}; // Initialize with zeros for this test

  String result = aes.decryptMessage(emptyInput, iv);

  TEST_ASSERT_EQUAL_STRING("", result.c_str());
}

void test_decryptMessage_invalid_base64_input()
{
  AssistentAES aes;
  const char *invalidBase64 = "!@#$%^&*()";
  byte iv[16] = {0};

  String result = aes.decryptMessage(invalidBase64, iv);

  TEST_ASSERT_EQUAL_STRING("", result.c_str());
}
void test_decryptMessage_with_special_characters()
{
  AssistentAES aes;
  const char *encryptedMessage = "U29tZSBzcGVjaWFsIGNoYXJhY3RlcnM6ICFAIyQlXiYqKCk="; // "Some special characters: !@#$%^&*()" encrypted
  byte iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

  String decryptedMessage = aes.decryptMessage(encryptedMessage, iv);

  TEST_ASSERT_EQUAL_STRING("Some special characters: !@#$%^&*()", decryptedMessage.c_str());
}

void test_decryptMessage_with_different_ivs()
{
  AssistentAES aes;
  const char *testMessage = "Hello, World!";
  byte testKey[16] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
  byte iv1[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
  byte iv2[16] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

  // Encrypt the message with IV1
  memcpy(aes.aesKey, testKey, sizeof(testKey)); // Копирование данных
  memcpy(aes.aesIv, iv1, 16);
  String encrypted1 = aes.encryptMessage(testMessage);

  // Encrypt the same message with IV2
  memcpy(aes.aesIv, iv2, 16);
  String encrypted2 = aes.encryptMessage(testMessage);

  // Extract the encrypted messages from the JSON-like strings
  int startPos1 = encrypted1.indexOf("\"message\":\"") + 11;
  int endPos1 = encrypted1.lastIndexOf("\"");
  String encryptedMessage1 = encrypted1.substring(startPos1, endPos1);

  int startPos2 = encrypted2.indexOf("\"message\":\"") + 11;
  int endPos2 = encrypted2.lastIndexOf("\"");
  String encryptedMessage2 = encrypted2.substring(startPos2, endPos2);

  // Decrypt both messages
  String decrypted1 = aes.decryptMessage(encryptedMessage1.c_str(), iv1);
  String decrypted2 = aes.decryptMessage(encryptedMessage2.c_str(), iv2);

  // Check if both decrypted messages match the original
  TEST_ASSERT_EQUAL_STRING(testMessage, decrypted1.c_str());
  TEST_ASSERT_EQUAL_STRING(testMessage, decrypted2.c_str());
}

void test_decryptMessage_with_incorrect_key()
{
  AssistentAES aes;
  const char *encryptedMessage = "SGVsbG8gV29ybGQ="; // Base64 encoded "Hello World"
  byte iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

  // Set an incorrect key
  byte incorrectKey[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  memcpy(aes.aesKey, incorrectKey, sizeof(incorrectKey)); // Копирование данных

  String result = aes.decryptMessage(encryptedMessage, iv);

  // The result should not be "Hello World" due to incorrect key
  TEST_ASSERT_NOT_EQUAL("Hello World", result.c_str());

  // The result should not be empty (as the function doesn't return empty strings)
  TEST_ASSERT_NOT_EQUAL(0, result.length());
}

void test_decryptMessage_consistency()
{
  const char *encryptedMessage = "SGVsbG8gV29ybGQ="; // Base64 encoded "Hello World"
  byte iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

  AssistentAES aes;

  String result1 = aes.decryptMessage(encryptedMessage, iv);
  String result2 = aes.decryptMessage(encryptedMessage, iv);
  String result3 = aes.decryptMessage(encryptedMessage, iv);

  TEST_ASSERT_EQUAL_STRING(result1.c_str(), result2.c_str());
  TEST_ASSERT_EQUAL_STRING(result2.c_str(), result3.c_str());
  TEST_ASSERT_EQUAL_STRING(result1.c_str(), result3.c_str());
}
void test_decrypt_message_unicode_handling()
{
  // Prepare test data
  const char *encryptedMessage = "SGVsbG8g8J+MiSBXb3JsZA=="; // Base64 encoded "Hello 🌉 World"
  byte iv[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};

  // Create an instance of AssistentAES
  AssistentAES aes;

  // Call the decryptMessage function
  String decryptedText = aes.decryptMessage(encryptedMessage, iv);

  // Check if the decrypted text contains the expected Unicode character
  TEST_ASSERT_TRUE(decryptedText.indexOf("🌉") != -1);

  // Check if the entire decrypted message is correct
  TEST_ASSERT_EQUAL_STRING("Hello 🌉 World", decryptedText.c_str());
}
void runTestDecrypt()
{
  RUN_TEST(test_decryptMessage_valid_message);
  RUN_TEST(test_decryptMessage_empty_input);
  RUN_TEST(test_decryptMessage_invalid_base64_input);
  RUN_TEST(test_decryptMessage_with_special_characters);
  RUN_TEST(test_decryptMessage_with_different_ivs);
  RUN_TEST(test_decryptMessage_with_incorrect_key);
  RUN_TEST(test_decryptMessage_consistency);
  RUN_TEST(test_decrypt_message_unicode_handling);
}