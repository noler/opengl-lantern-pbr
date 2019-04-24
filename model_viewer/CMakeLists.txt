cmake_minimum_required(VERSION 2.8.0)

# Specify project name
project(model_viewer)

# Specify build type
set(CMAKE_BUILD_TYPE RelWithDebInfo)

# Set extra compiler flags
if(UNIX AND NOT APPLE)
  set(CMAKE_CXX_FLAGS "-W -Wall -std=c++0x")
endif(UNIX AND NOT APPLE)
if(APPLE)
  set(CMAKE_CXX_FLAGS "-W -Wall -std=c++0x -ObjC++")
endif(APPLE)

# Add source directories
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/src" PROJECT_SRCS)

# Add include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/src")

# Define variable for linked libraries
set(PROJECT_LIBRARIES)

# GLFW
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw" ${CMAKE_CURRENT_BINARY_DIR}/glfw)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../external/glfw/include")

# OpenGL
find_package(OpenGL REQUIRED)
if(OPENGL_FOUND)
  include_directories(SYSTEM ${OPENGL_INCLUDE_DIR})
  set(PROJECT_LIBRARIES ${PROJECT_LIBRARIES} ${OPENGL_LIBRARIES})
endif(OPENGL_FOUND)

# GLEW
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/../external/glew/src" PROJECT_SRCS)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../external/glew/include")
add_definitions(-DGLEW_STATIC -DGLEW_NO_GLU)

# GLM
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../external/glm")

# lodepng
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/../external/lodepng" PROJECT_SRCS)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../external/lodepng")

# ImGui
aux_source_directory("${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui" PROJECT_SRCS)
include_directories(SYSTEM "${CMAKE_CURRENT_SOURCE_DIR}/../external/imgui")

# Add executable for project
add_executable(${PROJECT_NAME} ${PROJECT_SRCS})

# Link executable to libraries
target_link_libraries(${PROJECT_NAME} glfw ${PROJECT_LIBRARIES} ${GLFW_LIBRARIES})

# Install executable
install(TARGETS ${PROJECT_NAME} DESTINATION bin)
