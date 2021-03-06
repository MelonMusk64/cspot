#ifndef UTILS_H
#define UTILS_H

#include <unistd.h>
#include <vector>
#include "sys/socket.h"
#include <cstdint>
#include <netdb.h>
#include <cstring>
#include <memory>
#include <chrono>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#define HMAC_SHA1_BLOCKSIZE 64

std::vector<uint8_t> blockRead(int fd, size_t readSize);
unsigned long long getCurrentTimestamp();
ssize_t blockWrite (int fd, std::vector<uint8_t> data);

uint64_t hton64(uint64_t value);
unsigned char h2int(char c);
std::string urlDecode(std::string str);


std::string bytesToHexString(std::vector<uint8_t> &bytes);

// Reads a type from vector of binary data
template <typename T>
T extract(const std::vector<unsigned char> &v, int pos)
{
  T value;
  memcpy(&value, &v[pos], sizeof(T));
  return value;
}

// Writes a type to vector of binary data
template <typename T>
std::vector<uint8_t> pack(T data)
{
     std::vector<std::uint8_t> rawData( (std::uint8_t*)&data, (std::uint8_t*)&(data) + sizeof(T));

     return rawData;
}


#endif