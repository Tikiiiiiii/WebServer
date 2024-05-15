## 0.写在前面
风格是很重要的内容，在一个大型项目中，风格总是一致的，如果一个项目里的代码风格五花八门，那么令人阅读代码是十分头疼的事情，因此大型公司总是重视代码风格，树立一个规范来令员工参考，接下来介绍google 的c++ 代码规范常用精选

官方文档：http://google-styleguide.googlecode.com/   
中文官方文档：https://zh-google-styleguide.readthedocs.io/en/latest/google-cpp-styleguide/
## 1.头文件
### 1.2 #define防护符
头文件都应该使用#define来防止重复导入头文件，推介格式是项目_路径_文件名_H_
```cpp
#ifndef WEBSERVER_SRC_SERVER_H_
#define WEBSERVER_SRC_SERVER_H_
//...
#endif
```

### 1.6 #include导入顺序
推荐按照以下顺序导入头文件: 
- 配套的头文件
- C 语言系统库头文件
- C++ 标准库头文件
- 其他库的头文件
- 本项目的头文件

```cpp
//file:Server.cpp
#include "Server.h"
#include <stdio.h>
#include <iostream>
#include "ThreadPool.h"
//...
```

## 2.作用域
### 2.4 局部变量
应该尽可能缩小函数变量的作用域, 并在声明的同时初始化
```cpp
int someclass;
someclass=f();//不好

int otherclass = f(); //应该这么做

int jobs = NumJobs();
// 更多代码...
f(jobs);      // 不好: 初始化和使用位置分离

int jobs = NumJobs();
f(jobs);      // 良好: 初始化以后立即 (或很快) 使用.

//对于在if while for 等语句块中的变量，若该变量是非类对象，应该在块内声明，而类对象应该在块外声明，若在块内声明，频繁调用构造和析构函数是很费时的
Foo f;  // 调用 1 次构造函数和析构函数.
for (int i = 0; i < 1000000; ++i) {
    f.DoSomething(i);
}
```

## 3. 类
### 3.8 存取控制
将所有数据成员声明为 private,并对某些需要修改和访问的成员提供get和set函数

### 3.9 声明顺序
在各个部分中, 建议将类似的声明放在一起, 并且建议以如下的顺序:
- 类型及其别名 (包括 typedef, using, enum, 嵌套的结构体与类以及友元类型)
- (可选, 仅用于 struct) 非静态数据成员
- 静态常量
- 工厂函数
- 构造函数和赋值运算符
- 析构函数
- 所有的剩余函数 (静态与非静态成员函数, 以及友元函数)
- 所有的剩余数据成员 (静态的和非静态的)

## 4. 函数
### 4.2 函数长度
函数如果超过40行，就考虑能不能进一步分割

### 4.3 参数顺序
函数参数顺序：输入参数在前，输入能用const就const，输出参数在后。引用参数最好都加上const，如有改变的可能，使用指针更直白

### 4.4 类型后置声明方法
auto foo(int x) -> int;

## 5. 奇迹
### 5.1 智能指针
动态分配出的对象最好有单一且固定的所有主, 并通过智能指针传递所有权

## 6.C++特性
### 6.8 类型转换
使用 C++ 的类型转换, 如 static_cast<>(). 不要使用 int y = (int)x 或 int y = int(x) 等转换方式

### 6.10 前置自增和自减
对于迭代器和其他模板对象使用前缀形式 (++i) 的自增,自减运算符,因为他们比后缀形式的自增自减效率要高，常用于循环等不需要用到该变量的地方
```cpp
i++:{
  int temp=i; //需要执行一次拷贝构造
  i=i+1;
  return temp;
}

++i:{
  i=i+1;
  return i;
}
```

### 6.11 const修饰
我们强烈建议你在任何可能的情况下都要使用 const. 此外有时改用C++11推出的constexpr 更好。

### 6.16 空值
指针使用nullptr，字符使用'\0'

### 6.17 sizeof
尽可能用sizeof(varname)代替sizeof(type)
使用 sizeof(varname) 是因为当代码中变量类型改变时会自动更
sizeof(type) 可能不适用于某些情况
```cpp
Struct data; 
memset(&data, 0, sizeof(data));
//memset(&data, 0, sizeof(Struct));
```

### 6.18 auto
可以在不影响程序可读性的情况下使用auto去跳过烦琐的c++类型名，但别用在局部变量外的地方

### 6.20 lambda表达式
lambda表达式是一种匿名函数,它的格式如下
```cpp
// [] 内是要引用的外部值
// () 内是参数
// -> 后是返回值
// {} 内是函数内容
int a=0;
[a](int b)->int{
    return a+b;
}
```

##  7. 命名规则（重点）
### 7.1 通用命名规则
命名要有描述性,要让阅读代码的人能理解,少用缩写,但一些广为人知的缩写是允许的,如i表示迭代变量,T表示模板参数
```cpp
//正确写法
int price_count_reader;    // 无缩写
int num_errors;            // "num" 是一个常见的写法
int num_dns_connections;   // 人人都知道 "DNS" 是什么

//错误写法
int n;                     // 毫无意义.
int nerr;                  // 含糊不清的缩写.
int n_comp_conns;          // 含糊不清的缩写.
int wgc_connections;       // 只有贵团队知道是什么意思.
int pc_reader;             // "pc" 有太多可能的解释了.
int cstmr_id;              // 删减了若干字母.
```

### 7.2 文件命名
文件名全小写，以_隔开每个单词：   
my_project.cpp

### 7.3 类型名
单词首字母大写，不隔开
```cpp
// 类和结构体
class UrlTable { ... }
class UrlTableTester { ... }
struct UrlTableProperties { ... }

// 类型定义
typedef hash_map<UrlTableProperties *, string> PropertiesMap;

// using 别名
using PropertiesMap = hash_map<UrlTableProperties *, string>;

// 枚举
enum UrlTableErrors { ... }
```

### 7.4 变量命名
变量(包括参数)和数据成员名一律小写, 单词之间用下划线连接也可以不用，类的成员变量以下划线结尾, 但结构体的就不用
```cpp
string table_name;  // 好 - 用下划线.
string tablename;   // 好 - 全小写.

class TableInfo {
  ...
 private:
  string table_name_;  // 好 - 后加下划线.
  string tablename_;   // 好.
  static Pool<TableInfo>* pool_;  // 好.
};

struct UrlTableProperties {
  string name;
  int num_entries;
  static Pool<UrlTableProperties>* pool;
};
```

###  7.5 常量命名
k开头，大小写混合
```cpp
const int kDaysInWeek=7;
```

### 7.6 函数命名
单词首字母大写，没有连接符
```cpp
AddTableEntry()
DeleteUrl()
OpenFileOrDie()
```

### 7.7 命名空间命名
全小写
```cpp
namespace mywebserver{

}
```

### 7.9 宏命名
全大写
```cpp
#define MAX_COUNT 5
```

## 8.注释
注释要统一一种 /* */ 或者 // 
对每部分内容进行介绍功能
大项目在文件开头常常会有版权公告

## 9.格式
### 9.1 行长度
每一行代码字符数不超过 80

### 9.2 非ASCII字符
尽量不使用非 ASCII 字符, 使用时必须使用 UTF-8 编码

### 9.3 行首缩进
两个空格而不是tab
```cpp
  int a=0;
  int b=1;
  std::cout<<"Hello World";
```

### 9.4 函数声明与定义
返回类型和函数名在同一行, 参数也尽量放在同一行, 如果放不下就对形参分行

左圆括号 和 函数名 同行 且没有空格
圆括号与参数间没有空格

左大括号与最后一个参数同行, 不另起新行.
右大括号总是单独位于函数最后一行, 或者与左大括号同一行.

右圆括号和左大括号间总是有一个空格

所有形参应尽可能对齐,换行后的参数保持 4 个空格的缩进.

```cpp
ReturnType ClassName::FunctionName(Type par_name1, Type par_name2) {
  DoSomething();
  ...
}
```

### 9.6 函数调用
要么一行写完函数调用, 要么在圆括号里对参数分行, 要么参数另起一行且缩进四格. 如果没有其它顾虑的话, 尽可能精简行数
```cpp
bool retval = DoSomething(argument1, argument2, argument3);

if (...) {
  ...
  ...
  if (...) {
    DoSomething(
        argument1, argument2,  // 4 空格缩进
        argument3, argument4);
  }
}
```

### 9.8 条件语句
```cpp
if (condition) {  // 圆括号里没有空格 圆括号外有空格
  ...  // 2 空格缩进.
} else if (...) {  // else 与 if 的右括号同一行.
  ...
} else {
  ...
}

if (x == kFoo) return new Foo();//简短的条件语句允许写在同一行


// 不可以这样子 - IF 有大括号 ELSE 却没有.
if (condition) {
  foo;
} else
  bar;
// 不可以这样子 - ELSE 有大括号 IF 却没有.
if (condition)
  foo;
else {
  bar;
}
// 只要其中一个分支用了大括号, 两个分支都要用上大括号.
if (condition) {
  foo;
} else {
  bar;
}
```

### 9.9 循环和选择语句
```cpp
switch (var) {
  case 0: {  // 2 空格缩进
    ...      // 4 空格缩进
    break;
  }
  case 1: {
    ...
    break;
  }
  default: {
    assert(false);
  }
}
```

### 9.14 预处理指令
顶格 不缩进
```cpp
#include <iostream>
#define PI 3.14
```

### 9.15 访问控制顺序和格式
访问控制块的声明依次序是 public:, protected:, private:, 每个都缩进 1 个空格.
class MyClass : public OtherClass {
 public:      // 注意有一个空格的缩进
  MyClass();  // 标准的两空格缩进
  explicit MyClass(int var);
  ~MyClass() {}

  void SomeFunction();
  void SomeFunctionThatDoesNothing() {
  }

  void set_some_var(int var) { some_var_ = var; }
  int some_var() const { return some_var_; }

 private:
  bool SomeInternalFunction();

  int some_var_;
  int some_other_var_;
};

### 9.18 水平留白
 \* & -> . 和变量间不留白
```cpp
x = *p;
p = &x;
x = r.y;
x = r->y;
// * 和 & 可以前置空格或后置空格，但不要两者都有
// 好, 空格前置.
char *c;
const string &str;

// 好, 空格后置.
char* c;
const string& str;
```

比较运算左右留一个空格，逻辑运算符的断行要统一
```cpp
if (this_one_thing > this_other_thing &&
    a_third_thing == a_fourth_thing &&
    yet_another && last_one) {
  ...
}
```

```cpp
void f(bool b) {
//左大括号前总有空格
int i = 0;
//分号前不加空格
// 继承与初始化列表中的冒号前后恒有空格.
class Foo : public Bar {
 public:
  // 对于单行函数的实现, 在大括号内加上空格
  // 然后是函数实现
  Foo(int b) : Bar(), baz_(b) {}  // 大括号里面是空的话, 不加空格.
  void Reset() { baz_ = 0; }  // 用空格把大括号与实现分开.
//

if (b) {          // if 条件语句和循环语句关键字后均有空格.
} else {          // else 前后有空格.
}
while (test) {}   // 圆括号内部不紧邻空格.
switch (i) {
for (int i = 0; i < 5; ++i) {
switch ( i ) {    // 循环和条件语句的圆括号里可以与空格紧邻.
if ( test ) {     // 圆括号, 但这很少见. 总之要一致.
for ( int i = 0; i < 5; ++i ) {
for ( ; i < 5 ; ++i) {  // 循环里内 ; 后恒有空格, ;  前可以加个空格.
switch (i) {
  case 1:         // switch case 的冒号前无空格.
    ...
  case 2: break;  // 如果冒号有代码, 加个空格.
```

操作符前后都应有空格，但圆括号内部无空格，内部有，一元操作符不加空格

```cpp
x = 0;
v = w * x + z / (m - n);
++i;
x = -1;
x = !y;
```

### 9.19 垂直留白
垂直留白越少越好
两个函数定义之间的空行不要超过 2 行, 函数体首尾不要留空行, 函数体中也不要随意添加空行