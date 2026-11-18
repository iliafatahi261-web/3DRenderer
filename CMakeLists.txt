cmake_minimum_required(VERSION 3.18)

project(3DRenderer)

add_executable(${PROJECT_NAME} src/main.cpp)

target_include_directories(${PROJECT_NAME} PUBLIC include/)

target_link_libraries(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/lib/libraylib.a
	pthread
	X11
	GL
	m
	dl)

file(GLOB MY_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src.*.cpp")

target_sources(${PROJECT_NAME} PRIVATE ${MY_SOURCES})
