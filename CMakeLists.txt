cmake_minimum_required(VERSION 3.20)
project(LuaJITDecode VERSION 1.0.0 LANGUAGES CXX)

# C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Ensure PIC for shared library builds (important on some platforms)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# 收集源文件
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)

# 创建动态库
add_library(LuaJITDecode SHARED ${SOURCES} ${HEADERS})

# Export header include paths for consumers and mark the build to export symbols
target_include_directories(LuaJITDecode
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

# When building the library itself, define NATIVE_EXPORTS so the API macro exports symbols
target_compile_definitions(LuaJITDecode PRIVATE NATIVE_EXPORTS)

# Library properties
set_target_properties(LuaJITDecode PROPERTIES
    # Do not set VERSION/SOVERSION to avoid installing multiple versioned dylib files.
    # Keeping these empty makes CMake install only a single unversioned library file
    # (libLuaJITDecode.dylib) which is the desired behavior on this project.
    POSITION_INDEPENDENT_CODE ON
    PUBLIC_HEADER "main.h"
)

if(APPLE)
    # On macOS enable rpath and use @rpath for install_name so the dylib can be relocated
    set_target_properties(LuaJITDecode PROPERTIES MACOSX_RPATH ON INSTALL_NAME_DIR "@rpath")
endif()

# 头文件搜索路径
# target_include_directories(LuaJITDecode
#     PUBLIC
#         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
#         $<INSTALL_INTERFACE:include>
# )

# 安装规则
install(TARGETS LuaJITDecode
    LIBRARY DESTINATION lib   # .so / .dylib
    RUNTIME DESTINATION lib   # .dll
    ARCHIVE DESTINATION lib   # .lib / .a
    PUBLIC_HEADER DESTINATION include)

# 公开头文件（按需改）
# set(PUBLIC_HDRS
#     ${CMAKE_CURRENT_SOURCE_DIR}/main.h
#     # 可以继续加
# )
install(FILES ${PUBLIC_HDRS} DESTINATION include)

# 默认安装到项目/dist
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist" CACHE PATH "" FORCE)
endif()