cmake_minimum_required(VERSION 3.28.0)
project(minecraft)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

# Darn Windows Compilation
IF (WIN32)
	set (CMAKE_CXX_COMPILER "C:/MinGW/bin/g++.exe")
	set (CMAKE_C_COMPILER "C:/MinGW/bin/gcc.exe")
ENDIF()

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
set(GLFW_USE_WAYLAND ON CACHE INTERNAL "")

FetchContent_MakeAvailable(glfw)

# GLAD 
set(GLAD_VERSION 0.1.36)
FetchContent_Declare(
  glad 
	URL https://github.com/Dav1dde/glad/archive/refs/tags/v${GLAD_VERSION}.tar.gz
	FIND_PACKAGE_ARGS ${GLAD_VERSION}
)
FetchContent_MakeAvailable(glad)

# CGLM
set(CGLM_VERSION 0.9.1)
FetchContent_Declare(
	cglm
	URL https://github.com/recp/cglm/archive/refs/tags/v${CGLM_VERSION}.tar.gz
	FIND_PACKAGE_ARGS ${GLM_VERSION}
)
FetchContent_MakeAvailable(cglm)

# Our entry point
add_executable(${PROJECT_NAME} src/main.c)

# Installed libraries
target_link_libraries(${PROJECT_NAME} glfw glad cglm)

# Source files
file(GLOB PROJECT_SRC_FILES CONFIGURE_DEPENDS "src/*.h" "src/*.c" "src/**/*.h" "src/**/*.c")
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SRC_FILES})


# Properties...
set_target_properties(${PROJECT_NAME} PROPERTIES CMAKE_C_STANDARD 99)
