# 要求 CMake 的最低版本
cmake_minimum_required(VERSION 3.16)
# 定义项目名和语言（C）
project(realworld_lib_install C)

# 指定使用 C11 标准
set(CMAKE_C_STANDARD 11)
# 要求必须满足 C11，不允许自动降级
set(CMAKE_C_STANDARD_REQUIRED ON)

# 提供标准安装目录变量（如 bin/lib/include）
include(GNUInstallDirs)
# 提供生成 Config/Version 包文件的辅助函数
include(CMakePackageConfigHelpers)

# 生成共享库 libnetfmt_shared.so（输出名后续可保持默认）
add_library(netfmt_shared SHARED src/netfmt.c)
# 生成静态库 libnetfmt_static.a
add_library(netfmt_static STATIC src/netfmt.c)
# 生成可执行文件 net_tool
add_executable(net_tool src/main.c)

# 给 net_tool 打开常用警告
target_compile_options(net_tool PRIVATE -Wall -Wextra)
# net_tool 链接静态库 netfmt_static
target_link_libraries(net_tool PRIVATE netfmt_static)

# 给共享库设置公开头文件目录：
# BUILD_INTERFACE: 在当前工程构建时，头文件路径是源码目录的 include
# INSTALL_INTERFACE: 安装后给外部项目使用时，头文件路径是安装前缀下的 include
target_include_directories(netfmt_shared PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
# 静态库同理，导出给外部使用时也携带正确头文件路径
target_include_directories(netfmt_static PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# 安装三个目标到标准目录，并把它们加入导出集合 netfmtTargets
# RUNTIME: 可执行文件 -> bin
# LIBRARY: 动态库 -> lib
# ARCHIVE: 静态库 -> lib
install(TARGETS net_tool netfmt_shared netfmt_static
    # 记录这些目标到导出集合，后续写成 netfmtTargets.cmake
    EXPORT netfmtTargets
    # 可执行文件安装目录
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    # 动态库安装目录
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    # 静态库安装目录
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
# 安装公开头文件到 include
install(FILES include/netfmt.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# 把导出集合写成 netfmtTargets.cmake 并安装
# NAMESPACE netfmt:: 让外部用 netfmt::netfmt_static 这种名字链接
install(EXPORT netfmtTargets
    # 导出文件名
    FILE netfmtTargets.cmake
    # 导出目标统一前缀命名空间
    NAMESPACE netfmt::
    # 导出文件安装目录
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/netfmt
)

# 用模板生成 netfmtConfig.cmake（find_package 时会先找它）
configure_package_config_file(
    # 模板输入文件
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/netfmtConfig.cmake.in
    # 生成后的 Config 文件路径（在 build 目录）
    ${CMAKE_CURRENT_BINARY_DIR}/netfmtConfig.cmake
    # 将来安装时 Config 文件所在目录
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/netfmt
)
# 生成版本文件 netfmtConfigVersion.cmake（用于版本匹配）
write_basic_package_version_file(
    # 生成后的 Version 文件路径
    ${CMAKE_CURRENT_BINARY_DIR}/netfmtConfigVersion.cmake
    # 当前包版本
    VERSION 1.0.0
    # 兼容策略：主版本相同即兼容
    COMPATIBILITY SameMajorVersion
)
# 安装 Config 和 Version 文件到包目录
# 到这里外部项目就可以 find_package(netfmt CONFIG REQUIRED)
install(FILES
    # 安装 Config 文件
    ${CMAKE_CURRENT_BINARY_DIR}/netfmtConfig.cmake
    # 安装 Version 文件
    ${CMAKE_CURRENT_BINARY_DIR}/netfmtConfigVersion.cmake
    # 安装到 CMake 包目录
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/netfmt
)
