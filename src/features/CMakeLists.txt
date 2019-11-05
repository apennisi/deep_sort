function(featuregetter) 
  include_directories(${PROJECT_BINARY_DIR}/../src/feature/include/)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/feature/include/)
      
  file(GLOB_RECURSE FEATURE_SRC "src/feature/src/*")
  
  add_library(featuregetter SHARED ${FEATURE_SRC})
  target_link_libraries(featuregetter ${OpenCV_LIBS} TensorflowCC::Shared tensorflow_framework ${CUDA_LIBRARIES})
  
endfunction()