cmake_minimum_required(VERSION 3.1.6)
set(CMAKE_CXX_STANDARD 20)
project(cmake-sfml-opengl VERSION 0.1.0)

find_package(SFML 2.5.1 COMPONENTS system graphics audio network REQUIRED)

add_executable(cmake-sfml-opengl src/main.cpp src/graphics.cpp)
include_directories(${CMAKE_SOURCE_DIR}/inc)
include_directories(make-sfml-opengl PRIVATE /usr/include)
target_link_libraries(cmake-sfml-opengl PRIVATE sfml-audio sfml-graphics sfml-network sfml-system)
target_link_libraries(cmake-sfml-opengl PRIVATE GL)
target_link_libraries(cmake-sfml-opengl PRIVATE GLEW)

add_custom_command(
        TARGET cmake-sfml-opengl POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
                ${CMAKE_SOURCE_DIR}/resources/background.jpg
                ${CMAKE_CURRENT_BINARY_DIR}/background.jpg)
add_custom_command(
	TARGET cmake-sfml-opengl POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_SOURCE_DIR}/resources/sansation.ttf
			${CMAKE_CURRENT_BINARY_DIR}/sansation.ttf)
add_custom_command(
	TARGET cmake-sfml-opengl POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy
			${CMAKE_SOURCE_DIR}/resources/texture.jpg
			${CMAKE_CURRENT_BINARY_DIR}/texture.jpg)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

set_property(TARGET cmake-sfml-opengl PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build")