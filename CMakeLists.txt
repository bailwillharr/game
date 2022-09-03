cmake_minimum_required(VERSION 3.12)

# options

project(game LANGUAGES CXX
	VERSION "0.1.0"
)

add_executable(${PROJECT_NAME}

	src/main.cpp

	src/game.cpp
	src/game.hpp
	src/meshgen.cpp
	src/meshgen.hpp
	src/terrain.cpp
	src/terrain.hpp
	src/camera_controller.cpp
	src/camera_controller.hpp
	
 )

# compiling options:

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W3)
	target_compile_options(${PROJECT_NAME} PRIVATE /MP)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE src)

add_subdirectory(engine)
target_link_libraries(${PROJECT_NAME} PRIVATE engine)
target_include_directories(${PROJECT_NAME} PRIVATE engine/include)

# linking options:
set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE TRUE)
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	set_property(TARGET ${PROJECT_NAME} PROPERTY WIN32_EXECUTABLE FALSE)
endif()

add_custom_command(
	TARGET ${PROJECT_NAME} POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E create_symlink
	${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)