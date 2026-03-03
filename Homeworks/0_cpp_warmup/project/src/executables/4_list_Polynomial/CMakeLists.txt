add_executable(4_list_Polynomial main.cpp)   # 用于编译生成可执行文件

target_include_directories(4_list_Polynomial PUBLIC ${PROJECT_SOURCE_DIR}/include)  # 指定头文件路径

target_link_libraries(4_list_Polynomial PolynomialListLib)  # 指定链接库

set_target_properties(4_list_Polynomial PROPERTIES ${OUTPUT_PROP})  # 设置输出属性

# add_executable适用于生成可执行文件，而add_library适用于生成库文件。
# target_include_directories用于指定头文件的搜索路径。
# target_link_libraries用于指定链接库。
# 这里的PolynomialListLib是我们自己编写的库文件，我们需要先编译生成该库文件，然后再编译生成可执行文件。
# set_target_properties用于设置输出属性，如输出文件名、输出路径等。