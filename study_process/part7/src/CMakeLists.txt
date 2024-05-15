# 本文件用于创建核心库的动态链接库
# 检索所有源文件
file(GLOB_RECURSE source_files ${PROJECT_SOURCE_DIR}/src/*.cpp)
# 创建库
add_library(web_shared SHARED ${source_files})