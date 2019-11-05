cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0060 NEW)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3 -g")
project(deep_sort)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)

if(COMPILER_SUPPORTS_CXX14)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -pthread")
else()
  message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

find_package(OpenCV REQUIRED)

message(STATUS "OpenCV library status:")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

find_package(OpenMP REQUIRED)
if (OPENMP_FOUND)
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

find_package(Eigen3 REQUIRED)
find_package(Boost COMPONENTS system)
find_package(TensorflowCC COMPONENTS Shared)
find_package(CUDA)

include_directories(${EIGEN3_INCLUDE_DIR})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_BINARY_DIR}/../src/matching/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/matching/include/)
include_directories(${PROJECT_BINARY_DIR}/../src/thirdPart/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/thirdPart/include/)
include_directories(${PROJECT_BINARY_DIR}/../src/features/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/features/include/)
include_directories(${PROJECT_BINARY_DIR}/../src/app/include/)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/app/include/)

add_definitions(-Wuninitialized)
add_definitions(-Wreturn-type)
add_definitions(-Wsign-compare)
add_definitions(-Wuninitialized)
add_definitions(-Wunused-function)
add_definitions(-Wunused-label)    
add_definitions(-Wunused-value) 
add_definitions(-Wunused-variable)

set(EXECUTABLE_OUTPUT_PATH ../bin)
set(LIBRARY_OUTPUT_PATH ../lib)
file(GLOB_RECURSE SRC_FILES "src/app/src/*.cpp" "src/features/*.cpp" "src/matching/src/*.cpp" "src/thirdPart/src/*.cpp")


#Library
add_library(deepsort SHARED ${SRC_FILES})
target_link_libraries(deepsort ${OpenCV_LIBS}  ${Boost_LIBRARIES} TensorflowCC::Shared tensorflow_framework ${CUDA_LIBRARIES})

add_executable(deep_sort ${SRC_FILES})
target_link_libraries(deep_sort deepsort tensorflow_framework) #tensorflow_framework is required otherwise you get the error: "symbol _ZN10tensorflow27_SaverDef_default_instance_E version tensorflow not defined in file libtensorflow_cc.so.1 with link time reference"