/*

Buffer类：
echo时的缓冲区，封装和复用性

*/

#pragma once
#include <string>

#include "Macros.h"

class Buffer {
 public:
  Buffer();
  ~Buffer();

  DISALLOW_COPY_AND_MOVE(Buffer);

  void append(const char* _str, int _size);
  ssize_t size();
  const char* c_str();
  void clear();
  void getline();
  void setBuf(const char*);

 private:
  std::string buf;
};
