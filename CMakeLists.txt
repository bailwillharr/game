cmake_minimum_required(VERSION 3.12)

# options

project(game LANGUAGES CXX
	VERSION "0.1.0"
)

add_executable(${PROJECT_NAME}

	src/main.cpp

	src/game/game.cpp
	src/game/game.hpp
	src/game/meshgen.cpp
	src/game/meshgen.hpp
	src/game/terrain.cpp
	src/game/terrain.hpp
	src/game/camera_controller.cpp
	src/game/camera_controller.hpp
	
 )

# compiling options:

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD_REQUIRED ON)

if (MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE /W3)
	target_compile_options(${PROJECT_NAME} PRIVATE /M3)
endif()

target_include_directories(${PROJECT_NAME} PRIVATE src)

add_subdirectory(engine)
target_link_libraries(${PROJECT_NAME} PRIVATE engine)
target_include_directories(${PROJECT_NAME} PRIVATE engine/include)
