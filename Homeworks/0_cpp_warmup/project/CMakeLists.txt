# 这段CMake代码定义了项目的最低CMake版本要求，
# 并设置了项目的基本信息和输出目录，最后添加了子目录进行构建。
cmake_minimum_required(VERSION 3.22.1 FATAL_ERROR)
# 设置CMake的最低版本要求为3.22.1。
# 如果CMake的版本低于此版本，则构建过程会终止并显示错误信息。

project(CppPractices VERSION 1.0)
# 定义项目名称为CppPractices，版本号为1.0。
# PROJECT_NAME和PROJECT_VERSION变量会自动设置为这里的值。
message(STATUS "[Project] ${PROJECT_NAME} ${PROJECT_VERSION}")
# 输出一条状态信息，显示项目的名称和版本号。${PROJECT_NAME}
# 和${PROJECT_VERSION}是CMake自动设置的变量，分别代表项目名称和版本号。

include(cmake/helper.cmake)
# 包含cmake/helper.cmake文件，使得该文件中定义的函数和变量在当前CMake脚本中可用。

set(OUTPUT_PROP
    RUNTIME_OUTPUT_DIRECTORY_DEBUG          "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_SOURCE_DIR}/bin"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE        "${CMAKE_SOURCE_DIR}/bin"
)
# 开始定义一个名为OUTPUT_PROP的变量集合。这里的set命令用于设置变量的值，
# 后面跟着的多行内容是变量的属性设置。
# 设置调试模式下的可执行文件输出目录为${CMAKE_SOURCE_DIR}/bin。
# ${CMAKE_SOURCE_DIR}是CMake自动设置的变量，代表CMake源代码的根目录。
# 设置带调试信息的发布模式下的可执行文件输出目录为${CMAKE_SOURCE_DIR}/bin。
# 设置发布模式下的可执行文件输出目录为${CMAKE_SOURCE_DIR}/bin。

add_subdirectory(src)
# 添加src目录作为子目录进行构建。
# CMakeLists.txt文件会在src目录中查找另一个CMakeLists.txt文件，并执行其中的构建命令。





# cmake_minimum_required: 设置CMake的最低版本要求。
# FATAL_ERROR: 如果CMake版本低于指定版本，则终止构建并显示错误信息。
# project: 定义项目名称和版本号。
# VERSION: 指定项目的版本号。
# message: 输出一条信息到控制台。
# STATUS: 输出一条状态信息到控制台。
# [project]: 自定义的状态信息前缀。
# ${PROJECT_NAME}: 代表项目名称。
# ${PROJECT_VERSION}: 代表项目版本号。
# include: 包含一个CMake脚本文件。
# set: 设置一个变量的值。
# OUT_PROP: 定义一个变量集合，用于设置可执行文件输出目录。
# RUNTIME_OUTPUT_DIRECTORY_DEBUG: 调试模式下的可执行文件输出目录。
# RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO: 带调试信息的发布模式下的可执行文件输出目录。
# RUNTIME_OUTPUT_DIRECTORY_RELEASE: 发布模式下的可执行文件输出目录。
# ${CMAKE_SOURCE_DIR}: 代表CMake源代码的根目录。
# add_subdirectory: 添加一个子目录作为CMake项目的子目录进行构建。
