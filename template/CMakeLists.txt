cmake_minimum_required(VERSION 3.1)
project(tp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Search libraries
#find_package(OpenGL REQUIRED) # OpenGL
find_package(GLUT REQUIRED)
message("try to find GLM on System")
find_package(glm QUIET)
set(BUILD_SHARED_LIBS ON CACHE BOOL "build subdirectories with shared libs")
if (glm_FOUND)
	message("GLM system package found!")
else()
	message("GLM system package not found!")
	add_subdirectory(thirdparties/glm)
	set(glm_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/thirdparties/glm)
endif()
message("try to find Assimp on System")
find_package(assimp QUIET)
if (glm_FOUND)
	message("Assimp system package found!")
else()
	message("Assimp system package not found!")
	set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "")
	set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "")
	add_subdirectory(thirdparties/assimp)
	set(assimp_INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/thirdparties/assimp/include)
endif()
set(LIB_HEADERS ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} ${glm_INCLUDE_DIRS})
set(LIBS GL GLU GLEW ${GLUT_LIBRARIES} assimp)

# Add source files
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
# Add header files
file(GLOB_RECURSE HEADER_FILES
	${CMAKE_SOURCE_DIR}/include/*.h
	${CMAKE_SOURCE_DIR}/include/*.hpp)

# Use headers
include_directories(include/ /usr/include/ ${LIB_HEADERS} )
# Create executable
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})
# Link libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
