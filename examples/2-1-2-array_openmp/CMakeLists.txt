cmake_minimum_required(VERSION 2.8)
project(segments_intersection)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp)

OPTION(ENABLE_OPENMP "Enable openmp" ON) # Enabled by default
IF(ENABLE_OPENMP)
	message(STATUS "======== LOOKING FOR OPENMP ========================")
    ADD_DEFINITIONS(-DOPENMP)
    find_package(OpenMP)
	if (OPENMP_FOUND)
    	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	endif()
ENDIF(ENABLE_OPENMP)

# location of header files
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../../include
)

add_executable(main ${SOURCE_FILES})
