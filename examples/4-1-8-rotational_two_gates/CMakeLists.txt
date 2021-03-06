cmake_minimum_required(VERSION 2.8)
project(rotational_one_gate)

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

# python =======
set(REQUIRED_PYTHON_VERSION 3.4)
# message(STATUS "======== LOOKING FOR PYTHON ${REQUIRED_PYTHON_VERSION} ========================")
find_package ( PythonInterp ${REQUIRED_PYTHON_VERSION} REQUIRED)
find_package ( PythonLibs ${REQUIRED_PYTHON_VERSION} REQUIRED)

message(STATUS "PROJECT_SOURCE_DIR=${PROJECT_SOURCE_DIR}")

IF(PYTHONLIBS_FOUND)
  INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()
# end python ===

# location of header files
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../../include
)

add_executable(main ${SOURCE_FILES})


if(PYTHON_LIBRARIES)
  TARGET_LINK_LIBRARIES(main ${PYTHON_LIBRARIES})
ELSE()
  MESSAGE(FATAL_ERROR "Unable to link python libraries")
ENDIF()