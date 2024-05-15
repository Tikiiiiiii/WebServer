/*

Buffer类：
echo时的缓冲区，封装和复用性

*/

#pragma once
#include <string>

class Buffer
{
private:
    std::string buf;
public:
    Buffer();
    ~Buffer();
    void append(const char* _str, int _size);
    ssize_t size();
    const char* c_str();
    void clear();
    void getline();
    void setBuf(const char*);
};