/*

辅助程序的函数

*/

#pragma once

/*
判断所给的条件condition是否未真，若为真输出错误信息errmsg的内容并退出
*/
void errif(bool,const char *);

/*
把文件描述符改为非阻塞形式
*/
void setnonblocking(int);