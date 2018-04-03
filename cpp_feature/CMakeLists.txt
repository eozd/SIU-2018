cmake_minimum_required (VERSION 3.0)
project (cpp_feature)

if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

# set compiler
set (CMAKE_CXX_COMPILER g++)

# source directory
set (PROJECT_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set (PROJECT_TEST_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/test")

# source files
file(GLOB_RECURSE SOURCE_FILES_EXCEPT_MAIN "${PROJECT_SOURCE_DIR}/*.cpp")
list(REMOVE_ITEM SOURCE_FILES_EXCEPT_MAIN ${PROJECT_SOURCE_DIR}/main_feature.cpp)

# test source files
file(GLOB_RECURSE TEST_SOURCE_FILES "${PROJECT_TEST_SOURCE_DIR}/*.cpp")

# include directories
include_directories ("/usr/include" ,  # boost
					 "include" ,  # libraries in include folder
					 "src")  # make it easier to include files from other "modules"

# compiler flags
set (CMAKE_CXX_FLAGS "-std=c++14 \
					  -Wno-unused-result \
					  -Wsign-compare \
					  -march=x86-64 \
					  -mtune=generic \
					  -pipe \
					  -fstack-protector-strong \
					  -fwrapv \
					  -Wall -Wextra \
					  -Wno-missing-braces -Wno-unused-parameter \
					  -Wno-unused-function")

set (CMAKE_CXX_FLAGS_DEBUG "-g")
set (CMAKE_CXX_FLAGS_RELEASE "-O2")

# compile common code to a library to be used by different executables
set (SRC_LIB "src")
add_library(${SRC_LIB} STATIC ${SOURCE_FILES_EXCEPT_MAIN})

add_executable (feature "${PROJECT_SOURCE_DIR}/main_feature.cpp")
target_link_libraries(feature ${SRC_LIB})

set_target_properties(feature PROPERTIES RUNTIME_OUTPUT_DIRECTORY ..)

if (BUILD_TESTING)
	add_executable (tests ${TEST_SOURCE_FILES})
	target_link_libraries(tests ${SRC_LIB})
endif()
