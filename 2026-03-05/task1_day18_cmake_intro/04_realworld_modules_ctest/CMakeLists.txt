# 指定 CMake 最低版本要求
cmake_minimum_required(VERSION 3.16)
# 定义项目名和语言（C）
project(realworld_modules_ctest C)

# 统一 C 标准为 C11
set(CMAKE_C_STANDARD 11)
# 要求必须满足 C11
set(CMAKE_C_STANDARD_REQUIRED ON)

# 定义 mathx 库目标，源码来自 src/mathx.c
add_library(mathx src/mathx.c)
# 给 mathx 及其使用方暴露头文件目录 include
target_include_directories(mathx PUBLIC include)
# 对 mathx 开启常见警告
target_compile_options(mathx PRIVATE -Wall -Wextra)

# 定义 textx 库目标，源码来自 src/textx.c
add_library(textx src/textx.c)
# 给 textx 及其使用方暴露头文件目录 include
target_include_directories(textx PUBLIC include)
# 对 textx 开启常见警告
target_compile_options(textx PRIVATE -Wall -Wextra)

# 定义可执行程序 app
add_executable(app src/main.c)
# app 链接两个库：mathx 和 textx
target_link_libraries(app PRIVATE mathx textx)
# 对 app 开启常见警告
target_compile_options(app PRIVATE -Wall -Wextra)

# 启用 CTest 测试框架
enable_testing()

# 定义测试程序 test_mathx
add_executable(test_mathx tests/test_mathx.c)
# test_mathx 只需要链接 mathx
target_link_libraries(test_mathx PRIVATE mathx)
# 对 test_mathx 开启常见警告
target_compile_options(test_mathx PRIVATE -Wall -Wextra)
# 把 test_mathx 注册为 ctest 测试项
add_test(NAME test_mathx COMMAND test_mathx)

# 定义测试程序 test_textx
add_executable(test_textx tests/test_textx.c)
# test_textx 只需要链接 textx
target_link_libraries(test_textx PRIVATE textx)
# 对 test_textx 开启常见警告
target_compile_options(test_textx PRIVATE -Wall -Wextra)
# 把 test_textx 注册为 ctest 测试项
add_test(NAME test_textx COMMAND test_textx)
