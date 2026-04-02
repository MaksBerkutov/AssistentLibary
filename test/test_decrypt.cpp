#include <unity.h>
#include "AssistentAES.h"

static const String TEST_KEY = "00112233445566778899AABBCCDDEEFF";

void parseEncryptedPayload(const String &payload, String &ivHex, String &message)
{
  int ivStart = payload.indexOf("\"IV\":\"");
  int msgStart = payload.indexOf("\"message\":\"");

  TEST_ASSERT_TRUE_MESSAGE(ivStart >= 0, "IV field not found");
  TEST_ASSERT_TRUE_MESSAGE(msgStart >= 0, "message field not found");

  ivStart += 6;
  int ivEnd = payload.indexOf('"', ivStart);
  msgStart += 11;
  int msgEnd = payload.indexOf('"', msgStart);

  TEST_ASSERT_TRUE(ivEnd > ivStart);
  TEST_ASSERT_TRUE(msgEnd > msgStart);

  ivHex = payload.substring(ivStart, ivEnd);
  message = payload.substring(msgStart, msgEnd);
}

void decryptPayload(AssistentAES &aes, const String &payload, String &decrypted)
{
  String ivHex;
  String encryptedMessage;
  parseEncryptedPayload(payload, ivHex, encryptedMessage);

  byte iv[16];
  aes.hexStringToByteArray(ivHex, iv, 16);
  decrypted = aes.decryptMessage(encryptedMessage.c_str(), iv);
}

void test_encrypt_decrypt_roundtrip_ascii()
{
  AssistentAES aes;
  aes.begin(TEST_KEY);

  String source = "Hello, Assistent!";
  String encrypted = aes.encryptMessage(source);

  String decrypted;
  decryptPayload(aes, encrypted, decrypted);

  TEST_ASSERT_EQUAL_STRING(source.c_str(), decrypted.c_str());
}

void test_encrypt_decrypt_roundtrip_special_chars()
{
  AssistentAES aes;
  aes.begin(TEST_KEY);

  String source = "Some special characters: !@#$%^&*()";
  String encrypted = aes.encryptMessage(source);

  String decrypted;
  decryptPayload(aes, encrypted, decrypted);

  TEST_ASSERT_EQUAL_STRING(source.c_str(), decrypted.c_str());
}

void test_encrypt_decrypt_roundtrip_unicode()
{
  AssistentAES aes;
  aes.begin(TEST_KEY);

  String source = "Hello \xF0\x9F\x8C\x89 World";
  String encrypted = aes.encryptMessage(source);

  String decrypted;
  decryptPayload(aes, encrypted, decrypted);

  TEST_ASSERT_EQUAL_STRING(source.c_str(), decrypted.c_str());
}

void test_encrypt_same_message_with_different_ivs()
{
  AssistentAES aes;
  aes.begin(TEST_KEY);

  const char *source = "Hello, World!";
  byte iv1[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F};
  byte iv2[16] = {0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F};

  memcpy(aes.aesIv, iv1, 16);
  String encrypted1 = aes.encryptMessage(source);

  memcpy(aes.aesIv, iv2, 16);
  String encrypted2 = aes.encryptMessage(source);

  TEST_ASSERT_NOT_EQUAL(0, encrypted1.compareTo(encrypted2));

  String decrypted1;
  decryptPayload(aes, encrypted1, decrypted1);

  String decrypted2;
  decryptPayload(aes, encrypted2, decrypted2);

  TEST_ASSERT_EQUAL_STRING(source, decrypted1.c_str());
  TEST_ASSERT_EQUAL_STRING(source, decrypted2.c_str());
}

void test_decrypt_with_incorrect_key()
{
  AssistentAES aes;
  aes.begin(TEST_KEY);

  const char *source = "Hello, World!";
  String encrypted = aes.encryptMessage(source);

  byte wrongKey[16] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                       0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
  memcpy(aes.aesKey, wrongKey, sizeof(wrongKey));

  String decrypted;
  decryptPayload(aes, encrypted, decrypted);

  TEST_ASSERT_NOT_EQUAL(0, decrypted.compareTo(source));
}

void runTestDecrypt()
{
  RUN_TEST(test_encrypt_decrypt_roundtrip_ascii);
  RUN_TEST(test_encrypt_decrypt_roundtrip_special_chars);
  RUN_TEST(test_encrypt_decrypt_roundtrip_unicode);
  RUN_TEST(test_encrypt_same_message_with_different_ivs);
  RUN_TEST(test_decrypt_with_incorrect_key);
}
