#include "AssistentAES.h"
void AssistentAES::begin(String AesKey)
{
  this->hexStringToByteArray(AesKey, aesKey, 16);
}
String AssistentAES::GetIvString()
{
  String hexString = "";
  for (size_t i = 0; i < 16; i++)
  {
    if (aesIv[i] < 0x10)
    {
      hexString += "0";
    }
    hexString += String(aesIv[i], HEX);
  }
  hexString.toUpperCase();
  return hexString;
}
void AssistentAES::hexStringToByteArray(const String &hexString, byte *byteArray, size_t length)
{
  for (size_t i = 0; i < length; i++)
  {
    String byteString = hexString.substring(2 * i, 2 * i + 2);
    byteArray[i] = (byte)strtol(byteString.c_str(), nullptr, 16);
  }
}
String AssistentAES::decryptMessage(const char *encryptedMessage, byte *iv)
{
  int originalBytesLength = MYbase64::decodeLength(encryptedMessage);

  byte encryptedBytes[originalBytesLength];
  byte decryptedBytes[originalBytesLength];

  MYbase64::decode(encryptedMessage, encryptedBytes);

  aesLib.set_paddingmode((paddingMode)0);

  aesLib.decrypt(encryptedBytes, originalBytesLength,
                 decryptedBytes, aesKey, 16, iv);

  String decryptedText = String((char *)decryptedBytes);

  return decryptedText;
}
String AssistentAES::encryptMessage(String message)
{
  String IV_STRING = GetIvString();
  int bytesInputLength = message.length() + 1;

  byte bytesInput[bytesInputLength];

  message.getBytes(bytesInput, bytesInputLength);

  int outputLength = aesLib.get_cipher_length(bytesInputLength);

  byte bytesEncrypted[outputLength];

  aesLib.set_paddingmode((paddingMode)0);

  aesLib.encrypt(bytesInput, bytesInputLength, bytesEncrypted, aesKey, 16, aesIv);

  char base64EncodedOutput[MYbase64::encodeLength(outputLength)];

  MYbase64::encode(bytesEncrypted, outputLength, base64EncodedOutput);

  return "\"IV\":\"" + IV_STRING + "\",\"message\":\"" + String(base64EncodedOutput) + "\"";
}