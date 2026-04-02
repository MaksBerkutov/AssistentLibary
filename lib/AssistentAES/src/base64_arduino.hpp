#ifndef ASSISTENT_BASE64_ARDUINO_HPP
#define ASSISTENT_BASE64_ARDUINO_HPP

#include <stdint.h>
#include "base64.hpp"

namespace MYbase64
{
  inline int encodeLength(unsigned int inputLength)
  {
    return static_cast<int>(encode_base64_length(inputLength) + 1);
  }

  inline int decodeLength(const char *input)
  {
    return static_cast<int>(decode_base64_length(reinterpret_cast<const unsigned char *>(input)));
  }

  inline int encode(const uint8_t *input, unsigned int inputLength, char *output)
  {
    return static_cast<int>(encode_base64(
        reinterpret_cast<const unsigned char *>(input),
        inputLength,
        reinterpret_cast<unsigned char *>(output)));
  }

  inline int decode(const char *input, uint8_t *output)
  {
    return static_cast<int>(decode_base64(
        reinterpret_cast<const unsigned char *>(input),
        reinterpret_cast<unsigned char *>(output)));
  }
}

#endif
