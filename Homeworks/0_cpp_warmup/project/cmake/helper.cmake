function(AddExeWithFile exe_name)   
# 定义了一个名为AddExeWithFile的函数，该函数接受一个参数exe_name，这个参数代表要生成的可执行文件的名称。
  FILE(GLOB_RECURSE HEADER "*.h" )
  # 使用了FILE(GLOB_RECURSE HEADER "*.h" )命令递归地查找当前目录下的所有.h文件，
  # 并将这些文件的路径存储在变量HEADER中。
  FILE(GLOB_RECURSE SOURCE "*.cpp" )
  # 同样地，使用FILE(GLOB_RECURSESOURCE "*.cpp" )命令递归地查找当前目录下的所有.cpp文件，
  # 并将这些文件的路径存储在变量SOURCE中。
  message(STATUS "Header files: ${HEADER}")
  # 使用message(STATUS "Header files: ${HEADER}")命令输出一条状态信息，
  # 显示所有找到的.h文件的路径。
  message(STATUS "Source files: ${SOURCE}")
  # 使用message(STATUS "Source files: ${SOURCE}")命令输出一条状态信息，
  # 显示所找到的.cpp文件的路径。

  source_group("Headers" FILES ${HEADER})
  # 使用source_group("Headers" FILES ${HEADER})命令将所有.h文件分组，分组名称为"Headers"，
  # 这在IDE中可以方便地查看和管理文件。
  source_group("Sources" FILES ${SOURCE})
  # 使用source_group("Sources" FILES ${SOURCE})命令将所有.cpp文件分组，分组名称为"Sources"，
  # 同样用于在IDE中方便地查看和管理文件。

  add_executable(${exe_name} ${SOURCE} ${HEADER})
  # 使用add_executable (${exe_name} ${SOURCE} ${HEADER}) 命令根据提供的源文件和头文件列表创建一个可执行文件，
  # 可执行文件的名称由exe_name参数指定。
endfunction()


#  GLOBRECURSE : 递归地搜索当前目录及其子目录下的文件。
# FILE: 用于查找文件。
# HEADER: 所有.h文件的文件名列表。
# message: 输出一条状态信息。
# SOURCE: 所有.cpp文件的文件名列表。
# STATUS: 输出一条状态信息。
# source_group: 将文件分组。
# add_executable: 创建一个可执行文件。
# FILES: 指定要分组的文件列表。
# $(exe_name): 可执行文件的名称，由函数参数指定。 
# ${SOURCE}: 源文件列表。
# ${HEADER}: 头文件列表。


#----------------------------------------------------------

function(PrintAllVariables)
# 定义了一个名为PrintAllVariables的函数，
#  该函数不接受任何参数，其作用是打印所有CMake变量。 
#  这个函数可以帮助开发者调试CMake脚本，通过输出所有变量的名称和值来检查变量是否被正确设置。 

  get_cmake_property(_variableNames VARIABLES)
  # 使用get_cmake_property命令获取所有CMake变量的名称，并将这些名称存储在变量_variableNames中。
  # VARIABLES是一个属性名称，表示获取所有变量的列表。
  foreach(_variableName ${_variableNames})
  # 开始一个foreach循环，遍历_variableNames中的每一个变量名称，
  # 将当前处理的变量名称存储在_variableName中。
      message(STATUS "${_variableName}=${${_variableName}}")
      # 在每次循环中，使用message(STATUS ...)命令输出当前变量的名称及其值。
      # ${_variableName}是当前变量的名称，${${_variableName}}则是该变量的值。 

  endforeach()
  # 结束foreach循环。
endfunction()
# 结束函数定义。



# get_cmake_property: 获取CMake变量的属性。
# _variableNames: 所有CMake变量的名称。
# VARIABLES: 属性名称，表示获取所有变量的列表。
# foreach: 遍历所有CMake变量。
# _variableName: 当前处理的变量名称。
# ${_variableName}: 当前变量的名称。
# ${${_variableName}}: 当前变量的值。
# message: 输出一条状态信息。
# STATUS: 输出一条状态信息。
