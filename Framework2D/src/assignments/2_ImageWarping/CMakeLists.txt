project(2_ImageWarping)
file(GLOB source
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.h" 
  "${CMAKE_CURRENT_SOURCE_DIR}/warper/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/warper/*.h" 
)
add_executable(${PROJECT_NAME} ${source})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/_deps")
    target_include_directories(${PROJECT_NAME} PRIVATE _deps)
endif()
# 用于检查是否存在 _deps 目录，如果存在，则将其添加到编译器的包含路径中。这通常用于包含第三方库的头文件，以便在编译过程中能够找到这些库的定义和声明。

# Add Dlib support to the project (optional)
find_package(dlib QUIET)
if(dlib_FOUND)
  message(STATUS "Dlib found")
  target_link_libraries(${PROJECT_NAME} PRIVATE dlib::dlib)
  target_compile_definitions(${PROJECT_NAME} PRIVATE -DUSE_DLIB)
  # 这段代码的作用是检查是否找到了Dlib库，如果找到了，就将Dlib库链接到当前项目中，并定义一个预处理器宏USE_DLIB，以便在代码中可以使用条件编译来启用与Dlib相关的功能。
  else()
  message(STATUS "Dlib not found")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
  DEBUG_POSTFIX "_d"
  RUNTIME_OUTPUT_DIRECTORY "${BINARY_DIR}"
  LIBRARY_OUTPUT_DIRECTORY "${LIBRARY_DIR}"
  ARCHIVE_OUTPUT_DIRECTORY "${LIBRARY_DIR}") 
target_link_libraries(${PROJECT_NAME} PUBLIC common) 
target_compile_definitions(${PROJECT_NAME} PRIVATE -DDATA_PATH="${FRAMEWORK2D_DIR}/../Homeworks/2_image_warping/data")