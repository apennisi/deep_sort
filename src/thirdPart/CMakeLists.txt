function(thirdpart) 
  include_directories(${PROJECT_BINARY_DIR}/../src/thirdPart/)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/thirdPart/)
  #add_definitions(-DUSE_FACE_NET)
    
  file(GLOB_RECURSE THIRD_PART_SRC "src/thirdPart/*.cpp" "src/thirdPart/munkres/*.cpp" "src/thirdPart/munkres/adapters*.cpp")
  
  add_library(thirdpart SHARED ${THIRD_PART_SRC})
  target_link_libraries(thirdpart ${Boost_LIBRARIES})
  
endfunction()