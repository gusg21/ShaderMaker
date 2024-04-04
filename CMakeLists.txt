cmake_minimum_required(VERSION 3.21)
project(ShaderMaker)
set(CMAKE_CXX_STANDARD 17)

# Include guards
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(
            FATAL_ERROR
            "In-source builds not allowed. Please make a new directory (called a build directory) and run CMake from there."
    )
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(cmake/CPM.cmake)

# Main Executable
#file(GLOB ${PROJECT_NAME}_SRC src/*.cpp)
add_executable(ShaderMaker
        src/external/glad.cpp
        src/main.cpp
        src/shader.cpp)

# OpenGL
find_package(OpenGL REQUIRED)
message("OpenGL Include Dir: ${OPENGL_INCLUDE_DIR}")
message("OpenGL Libraries: ${OPENGL_LIBRARIES}")
#target_include_directories(ShaderMaker PUBLIC ${OPENGL_INCLUDE_DIR})
#target_link_libraries(ShaderMaker PUBLIC ${OPENGL_LIBRARIES})

# Assimp
set(ASSIMP_INSTALL OFF)
set(BUILD_SHARED_LIBS OFF)
CPMAddPackage(
        NAME "assimp"
        URL "https://github.com/assimp/assimp/archive/refs/tags/v5.3.1.zip"
        OPTIONS (
            "ASSIMP_BUILD_SAMPLES OFF" "ASSIMP_BUILD_ZLIB ON" "BUILD_SHARED_LIBS OFF"
            "ASSIMP_BUILD_TESTS OFF"
            "ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF"
            "ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF"
            "ASSIMP_BUILD_FBX_IMPORTER ON"
            "ASSIMP_BUILD_OBJ_IMPORTER ON"
            "ASSIMP_BUILD_COLLADA_IMPORTER ON"
        )
)
#target_compile_options(assimp PRIVATE -Wno-dangling-reference)
set (assimp_INCLUDE_DIR ${assimp_SOURCE_DIR}/include)
target_include_directories(ShaderMaker PRIVATE ${assimp_INCLUDE_DIR})
target_link_libraries(ShaderMaker PRIVATE assimp)

# SDL
CPMAddPackage("gh:libsdl-org/SDL#release-2.30.2")
target_link_libraries(ShaderMaker PRIVATE SDL2::SDL2main)
target_link_libraries(ShaderMaker PRIVATE SDL2::SDL2)
if (WIN32)
    add_custom_command(
            TARGET ShaderMaker POST_BUILD
            COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL2::SDL2>" "$<TARGET_FILE_DIR:ShaderMaker>"
            VERBATIM
    )
endif()

# Asset Bundle
add_custom_target(ShaderMaker_Assets ALL COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/assets/
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/assets/)
add_dependencies(ShaderMaker ShaderMaker_Assets)