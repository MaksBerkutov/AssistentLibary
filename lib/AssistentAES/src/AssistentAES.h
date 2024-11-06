#ifndef AssistentAES_H
#define AssistentAES_H
#include "Arduino.h"
#include "AESLib.h"
#include "arduino_base64.hpp"

class AssistentAES
{

public:
  AESLib aesLib;
  byte *aesKey = new byte[16]; //= {48, 85, 92, 36, 73, 111, 127, 18, 64, 38, 54, 89, 72, 105, 78, 15};
  byte aesIv[16] = {145, 33, 51, 67, 24, 173, 52, 209, 63, 42, 13, 115, 220, 3, 8, 10};
  void begin(String AesKey);
  String GetIvString();
  void hexStringToByteArray(const String &hexString, byte *byteArray, size_t length);
  String decryptMessage(const char *encryptedMessage, byte *iv);
  String encryptMessage(String message);
};

#endif