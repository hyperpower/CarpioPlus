cmake_minimum_required(VERSION 2.8)
project(carpio_test)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/build/)

set(SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp)

find_package(GTest)

find_package(MPI)
if (MPI_LIBRARY)
	set(MPI_INCLUDE_DIR ${MPI_INCLUDE_PATH} CACHE STRING "MPI Include Path")
	set(MPI_LIBRARIES ${MPI_LIBRARY};${MPI_EXTRA_LIBRARY} CACHE STRING "MPI Libraries")
	message(STATUS "MPI LIB = ${MPI_LIBRARIES}")
	message(STATUS "MPI INC = ${MPI_INCLUDE_PATH}")
endif (MPI_LIBRARY)

#OPTION(ENABLE_OPENMP "Enable openmp" ON) # Enabled by default
IF(ENABLE_OPENMP)
	message(STATUS "======== LOOKING FOR OPENMP ========================")
    ADD_DEFINITIONS(-DOPENMP)
    find_package(OpenMP)
	if (OPENMP_FOUND)
    	set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    	set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
	endif()
ENDIF(ENABLE_OPENMP)



#set(Python_ADDITIONAL_VERSIONS 3.5)
# Check the architecture and set CMAKE_LIBRARY_ARCHITECTURE accordingly
#if(UNIX)
#  if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#    set(CMAKE_LIBRARY_ARCHITECTURE "x86_64-linux-gnu")
#  endif()
#endif()
set(Python_ADDITIONAL_VERSIONS 3.4)
# python =======
set(REQUIRED_PYTHON_VERSION 3.4)
message(STATUS "======== LOOKING FOR PYTHON ${REQUIRED_PYTHON_VERSION} ========================")
find_package ( PythonInterp ${REQUIRED_PYTHON_VERSION} REQUIRED)
find_package ( PythonLibs   ${REQUIRED_PYTHON_VERSION} REQUIRED)


message(STATUS "PYTHON_EXECUTABLE:FILEPATH=${PYTHON_EXECUTABLE}")
message(STATUS "PYTHON_LIBRARY:FILEPATH=${PYTHON_LIBRARY}")
message(STATUS "PYTHON_LIBRARIES:=${PYTHON_LIBRARIES}")
message(STATUS "PYTHON_INCLUDE_DIR:FILEPATH=${PYTHON_INCLUDE_DIR}")
#message(STATUS "PYTHON_FRAMEWORK_INCLUDES=${PYTHON_FRAMEWORK_INCLUDES}")
message(STATUS "PYTHONLIBS_VERSION_STRING=${PYTHONLIBS_VERSION_STRING}")
#message(STATUS "PYTHON_FRAMEWORKS=${Python_FRAMEWORKS}")

IF(PYTHONLIBS_FOUND)
  INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIRS}")
ELSE()
  MESSAGE(FATAL_ERROR "Unable to find PythonLibs.")
ENDIF()

find_package(Threads REQUIRED)
if(THREADS_HAVE_PTHREAD_ARG)
  set_property(TARGET carpio_test PROPERTY COMPILE_OPTIONS "-pthread")
  set_property(TARGET carpio_test PROPERTY INTERFACE_COMPILE_OPTIONS "-pthread")
endif()

# end python ===
# location of header files
INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}/../include
)

INCLUDE_DIRECTORIES(${MPI_INCLUDE_PATH})

#find_package(VTK REQUIRED)
#include(${VTK_USE_FILE})

add_executable(carpio_test ${SOURCE_FILES})

# link the libraries to the executable
target_link_libraries (carpio_test pthread)
target_link_libraries (carpio_test gtest)
target_link_libraries (carpio_test ${MPI_LIBRARIES})

if(PYTHON_LIBRARIES)
  TARGET_LINK_LIBRARIES(carpio_test ${PYTHON_LIBRARIES})
ELSE()
  MESSAGE(FATAL_ERROR "Unable to link python libraries")
ENDIF()

if(CMAKE_THREAD_LIBS_INIT)
  TARGET_LINK_LIBRARIES(carpio_test "${CMAKE_THREAD_LIBS_INIT}")
endif()

#if(VTK_LIBRARIES)
#    target_link_libraries(carpio ${VTK_LIBRARIES})
#else()
#    target_link_libraries(carpio vtkHybrid vtkWidgets)
#endif()
