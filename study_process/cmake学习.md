## 1.CMake概述
CMake是一个**开源、跨平台**的编译、测试和打包工具，它使用比较简单的语言描述编译、安装的过程，输出Makefile或者project文件，再去执行构建。大多数IDE都集成CMake，相比于makefile，CMake不需要依赖当前编译的平台，并且工作量相对较少，依赖关系更少出错。

CMake过程如下所示，其先生成Makefile文件再生成目标文件：
![](1.png)

CMake的优点如下：
    可拓展性
    跨平台
    管理大型项目
    简化编译链接过程
## 2.CMake环境及安装
Linux环境：
    Ubuntu22.04
    CMake3.22.1

查看CMake版本：
>cmake --version

安装CMake：
>sudo apt-get install cmake

## 3.CMake的使用
### 3.1 CMake的创建
CMake编译所需要的文件(只能是这个名字，大小写不可改变)
>CMakeLists.txt

### 3.2 注释
使用#进行单行注释，#[[ ]]进行多行注释
```cmake
#这是一个CMake单行注释

#[[
这是一个
多行注释
]]
```

### 3.3 指定CMake版本
```cmake
cmake_minimum_required(VERSION 3.0) #指定最低版本（可选）
```

### 3.4 指定工程名
project定义项目名称，所支持语言等等
```cmake
#[[
project语法：
    project(<项目名> [<项目描述>] [<Web主页地址>] [<语言>])
    []可以省略
]]
cmake_minimum_required(VERSION 3.0) 
project(mywb)
```

### 3.5 定义生成文件
add_executable
```cmake
#[[
add_executable语法：
    add_executable(可执行文件名 源文件名)
    源文件名可以用空格" "或分号";"隔开
]]
add_executable(target src1.cpp src2.cpp src3.cpp)
```

### 3.6 CMake命令的执行
先用CMake生成Makefile文件，再执行make
>cmake CMakeLists.txt所在路径    
make 

### 3.7 变量
set
```cmake
#[[
定义语法：
set(变量名 值 可选项)
    变量名要唯一
    多值用" "或";"隔开
    值是以字符串类型存储到变量名中的

使用语法：
${变量名}
]]
set(SRC_LIST src1.cpp src2.cpp src3.cpp)
add_executable(target ${SRC_LIST})
```

### 3.8 指定c++使用标准
1.设置全局变量CMAKE_CXX_STANDARD
```cmake
#增加-std=c++11
set(CMAKE_CXX_STANDARD 11)
#增加-std=c++14
set(CMAKE_CXX_STANDARD 14)
#增加-std=c++17
set(CMAKE_CXX_STANDARD 17)
```
2.执行CMake时使用-DCMAKE_CXX_STANDARD=xx选项
>#增加-std=c++11   
cmake CMakeLists.txt文件路径 -DCMAKE_CXX_STANDARD=11   
#增加-std=c++14   
cmake CMakeLists.txt文件路径 -DCMAKE_CXX_STANDARD=14   
#增加-std=c++17  
cmake CMakeLists.txt文件路径 -DCMAKE_CXX_STANDARD=17    

### 3.9 指定输出路径
设置全局变量EXECUTABLE_OUTPUT_PATH
```cmake
set(EXECUTABLE_OUTPUT_PATH 输出路径)
#如果这个路径中的子目录不存在，会自动生成，无需自己手动创建
#动态库也会生成在这
```

### 3.10 搜索文件
1.使用aux_source_directory 命令可以查找某个路径下的所有源文(.c 或 .cpp)
```cmake
#aux_source_directory(搜索路径 存储变量名)
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src SRC_LIST)
#CMAKE_CURRENT_SOURCE_DIR 全局变量表示CMakeLists.txt所在路径
```
2.使用file搜索指定文件
```cmake
#file(GLOB/GLOB_RECURSE 存储变量名 搜索路径及文件类型)
#GLOB: 仅在指定目录下搜索
#GLOB_RECURSE：递归搜索
file(GLOB SRC ${PROJECT_SOURCE_DIR}/src/*.cpp)
# PROJECT_SOURCE_DIR 是 cmake命令后跟的路径
```

### 3.11 包含头文件
```cmake
include_directories(头文件路径)
```

### 3.12 制作库文件
```cmake
add_library(库名称 STATIC/SHARED 源文件 [源文件2] ...)
# STATIC：静态库 SHARED：动态库  
```
指定库输出路径
```cmake
set(LIBRARY_OUTPUT_PATH 输出路径)
```

### 3.13 链接库文件
#### 3.13.1 链接静态库
指定自定义链接文件所在的路径
```cmake
link_directories(库路径)
# 注意是路径而不是库名
```
链接静态库
```cmake
link_libraries(库名1 库名2)
# 系统静态库不需要指定路径 自定义静态库需要
```
#### 3.13.2 链接动态库
```cmake
add_executable(app ${SRC_LIST})
target_link_libraries(目标文件 访问权限 库名1 访问权限 库名2)
# 系统静态库不需要指定路径 自定义静态库需要 同上
# 访问权限
# PUBLIC：库具有传递性，由目标文件链接得到的目标文件仍可以使用库文件
# PRIVATE：不具有传递性，目标文件可以使用库，但其传递的目标文件不能使用该库文件
# INTERFACE：非传递性，并且目标文件仅有库文件函数名（即只知道接口但无法得到具体内容）
# target_link_libraries要在目标文件生成命令之后使用
```

### 3.14 日志
```cmake
message(消息等级 "消息内容" ...)
#[[
(无) ：重要消息
STATUS ：非重要消息
WARNING：警告, 会继续执行
AUTHOR_WARNING：警告 (dev), 会继续执行
SEND_ERROR：错误, 会继续执行，跳过生成的步骤
FATAL_ERROR：错误, 终止
]]
```

### 3.15 变量操作
```cmake
# 1.set
set(目标变量名 ${变量名1} ${变量名2} ...)

# 2.list添加到末尾
list(APPEND 待操作变量 ${变量名}/"字符串" ...)

# 3.移除某个元素
list(REMOVE_ITEM 待操作变量 ${变量名}/"字符串" ...)

# 4.获取子串个数
list(LENGTH 待操作变量 输出变量)

# 5.获取指定索引的元素
list(GET 待操作变量 索引1 索引2 ... 输出变量)
#索引从0开始编号，也可以是负数，-1表示列表的最后一个元素 
#索引超过列表的长度会报错

# 6.用指定连接符将列表中的元素连接起来组成一个字符串
list (JOIN 待操作变量 连接符 输出变量)
<list>：当前操作的列表
<glue>：指定的连接符（字符串）
<output variable>：新创建的变量，存储返回的字符串

# 7.查找列表是否存在指定的元素，若果未找到，返回-1
list(FIND 待操作变量 搜索元素 输出变量)

# 8.在指定的位置插入若干元素
list(INSERT 待操作变量 索引 元素1 元素2 ...)

# 9.将元素插入到列表的0索引位置
list (PREPEND 待操作变量 元素 ...)

# 10.将列表中最后元素移除
list (POP_BACK 待操作变量 输出变量 ...)

# 11.将列表中第一个元素移除
list (POP_FRONT 待操作变量 输出变量 ...)

# 12.将指定索引的元素从列表中移除
list (REMOVE_AT 待操作变量 索引1 索引2 ...)

# 13.移除列表中的重复元素
list (REMOVE_DUPLICATES 待操作变量)

# 14.列表翻转
list(REVERSE 待操作变量)

# 15.列表排序
list (SORT 待操作变量 [COMPARE <compare>] [CASE <case>] [ORDER <order>])
#[[COMPARE:指定排序方法。有如下几种值可选：
    STRING:按照字母顺序进行排序，默认
    FILE_BASENAME:如果是一系列路径名，会使用basename进行排序
    NATURAL:使用自然数顺序排序
CASE:指明是否大小写敏感。
    SENSITIVE:默认
    INSENSITIVE:按照大小写不敏感方式进行排序
ORDER:指定升降序。是升序
    ASCENDING:默认
    DESCENDING:降序
]]
```

### 3.16 宏的自定义、
在项目开发过程中，常常需要宏来帮助我们调试程序，但实际应用项目内容的时候是不需要这些宏的，因此需要在调试时定义宏，使得某些调试内容生效，而在真正生成时不使用宏使得调试信息移出项目。
```c
#include <stdio.h>
int main(){
#ifdef DEBUG
    printf("调试信息\n");
#endif
    return 0; 
}
```
定义方式：
```cmake
add_definitions(-D宏名称)
```
### 3.17 更多内容
官网文档：    
[cmake官方文档](https://cmake.org/cmake/help/latest/)

## 4.CMake的嵌套
CMake可以嵌套使用CMakeLists.txt文件，在嵌套使用中，子文件可以用父文件的变量，而反过来不行
CMake在父节点中添加子节点的方式为：
```cmake
add_subdirectory(子目录路径 [binary_dir] [EXCLUDE_FROM_ALL])
#后两者可以省略
``` 

## 5.CMake的流程控制
### 5.1 CMake条件判断 
```cmake
if(<condition>)
  <commands>
elseif(<condition>) # 可选, 可以重复
  <commands>
else()              # 可选
  <commands>
endif()
```

### 5.2 条件表达式condition
condition  
如果是1, ON, YES, TRUE, Y, 非零值，非空字符串时，condition返回True
如果是0, OFF, NO, FALSE, N, IGNORE, NOTFOUND，空字符串时，condition返回False

### 5.3 逻辑操作
```cmake
if(NOT <condition>) #取反

if(<condition1> AND <condition2>) #与

if(<condition1> OR <condition2>) #或
```

### 5.4 比较
#### 5.4.1基于数值的比较
```cmake
if(<variable|string> LESS <variable|string>) #小于
if(<variable|string> GREATER <variable|string>) #大于
if(<variable|string> EQUAL <variable|string>) #等于
if(<variable|string> LESS_EQUAL <variable|string>) #小于等于
if(<variable|string> GREATER_EQUAL <variable|string>) #大于等于
```
#### 5.4.2基于字符串比较
```cmake
if(<variable|string> STRLESS <variable|string>)
if(<variable|string> STRGREATER <variable|string>)
if(<variable|string> STREQUAL <variable|string>)
if(<variable|string> STRLESS_EQUAL <variable|string>)
if(<variable|string> STRGREATER_EQUAL <variable|string>)
```

### 5.5foreach循环
```cmake
# [0,n]循环 左闭右闭区间
foreach(存储循环次数变量i RANGE 次数n)
    message(STATUS ${i})
endforeach()

# [start,stop,step]循环
# 同python,从start开始stop结束，每次移动step步长
foreach(存储循环次数变量i RANGE start stop step)
    message(STATUS ${i})
endforeach()

# 遍历变量或者元素
foreach(<loop_var> IN [LISTS [<lists>]] [ITEMS [<items>]])
```

### 5.6while循环
```cmake
while(<condition>)
    <commands>
endwhile()
```

## 6.文件操作
```cmake
#判断文件或者目录是否存在
if(EXISTS 文件或目录)

#判断是不是目录
if(IS_DIRECTORY 绝对路径)

#判断是不是软连接
if(IS_SYMLINK 绝对路径/文件名)

#判断是不是绝对路径
if(IS_ABSOLUTE 绝对路径)