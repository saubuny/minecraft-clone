cmake_minimum_required(VERSION 3.28.0)
project(terraria)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# GLFW
set(GLFW_VERSION 3.3.8)
FetchContent_Declare(
  glfw 
	URL https://github.com/glfw/glfw/archive/refs/tags/${GLFW_VERSION}.tar.gz
  FIND_PACKAGE_ARGS ${GLFW_VERSION}
)

set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "")
set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "")
set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "")

set(GLFW_USE_WAYLAND OFF CACHE INTERNAL "")

FetchContent_MakeAvailable(glfw)

# GLAD 
set(GLAD_VERSION 0.1.36)
FetchContent_Declare(
  glad 
	URL https://github.com/Dav1dde/glad/archive/refs/tags/v${GLAD_VERSION}.tar.gz
	FIND_PACKAGE_ARGS ${GLAD_VERSION}
)

FetchContent_MakeAvailable(glad)

# Our entry point
add_executable(${PROJECT_NAME} src/main.c)

# Installed libraries
target_link_libraries(${PROJECT_NAME} glfw glad)

# Add personal libraries here

# add_library(testlib STATIC src/testlib.c include/test/testlib.h)
# target_include_directories(testlib PUBLIC include)
# target_link_libraries(${PROJECT_NAME} PUBLIC testlib)

# Properties...
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_C_STANDARD 99)
