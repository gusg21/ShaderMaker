cmake_minimum_required(VERSION 3.27)
project(ShaderMaker)
include(cmake/CPM.cmake)
set(CMAKE_CXX_STANDARD 17)

# Main Executable
#file(GLOB ${PROJECT_NAME}_SRC src/*.cpp)
add_executable(ShaderMaker
        src/main.cpp
        src/shader.cpp)

# OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(ShaderMaker PRIVATE ${OPENGL_INCLUDE_DIR})
target_link_libraries(ShaderMaker PRIVATE ${OPENGL_LIBRARIES})

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