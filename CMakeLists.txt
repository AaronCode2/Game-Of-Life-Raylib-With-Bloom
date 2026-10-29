cmake_minimum_required(VERSION 3.10...3.50)

project(Game_Of_Life)

include(FetchContent)

FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/6.0.zip
)

FetchContent_Declare(
    raylib_nuklear
    URL https://github.com/RobLoach/raylib-nuklear/archive/refs/tags/v6.0.1.zip
)

FetchContent_Declare(
    json
    URL https://github.com/nlohmann/json/archive/refs/tags/v3.12.0.zip
)

set(BUILD_EXAMPLES OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(raylib raylib_nuklear json)

set(
SOURCE_FILES

    src/main.cpp
    src/grid.cpp
    src/simulation.cpp
    src/renderer/renderer.cpp
    src/utility/assets.cpp
    src/utility/utils.cpp
)

add_executable(Game_Of_Life ${SOURCE_FILES})
target_link_libraries(Game_Of_Life raylib raylib_nuklear nlohmann_json::nlohmann_json)

if(WIN32)

    file(COPY "libs/renderdoc/renderdoc.dll" DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    # Thirdparty library for debugging
elseif(APPLE)

    target_link_options(Game_Of_Life PRIVATE $<$<CONFIG:Release>:-MACOSX_BUNDLE>)
endif()

target_compile_definitions(

    Game_Of_Life PUBLIC 
    $<$<CONFIG:Debug>:DEBUG_ENABLED>
    $<$<CONFIG:RelWithDebInfo>:DEBUG_ENABLED>
)

message("Operation Complete! Configured Correctly!")

if("${PLATFORM}" STREQUAL "Web")

    message(FATAL_ERROR "Do not build in this project in the Web as it doesn't work!")
endif()

if(APPLE)

    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()