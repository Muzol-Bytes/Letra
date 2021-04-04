cmake_minimum_required(VERSION 3.7)

project(Letra VERSION 1.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/bin/$<CONFIG>")
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH}) 

add_executable(${PROJECT_NAME}
    src/editor/command_prompt.cpp
    src/editor/cursor.cpp
    src/application.cpp
    src/graphics/rectangle.cpp
    src/graphics/drawable.cpp
    src/graphics/text.cpp
    src/graphics/renderer.cpp
    src/editor/buffer.cpp
    src/log/log.cpp
    src/file.cpp
    src/window/window.cpp
    src/main.cpp
    )

target_link_libraries(${PROJECT_NAME} d2d1.lib Dwrite.lib)
