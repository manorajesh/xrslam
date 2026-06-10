if(NOT TARGET depends::opencv)
  if(NOT OPENCV_ROOT)
    message(FATAL_ERROR "iOS builds require OPENCV_ROOT pointing at a custom OpenCV install")
  endif()

  find_path(OpenCV_INCLUDE_DIR
    NAMES opencv2/opencv.hpp
    PATHS "${OPENCV_ROOT}/include/opencv5" "${OPENCV_ROOT}/include/opencv4" "${OPENCV_ROOT}/include"
    NO_DEFAULT_PATH
    NO_CMAKE_FIND_ROOT_PATH
  )
  find_library(OpenCV_WORLD_LIB
    NAMES opencv_slamkit opencv_world opencv_world500
    PATHS "${OPENCV_ROOT}/lib"
    NO_DEFAULT_PATH
    NO_CMAKE_FIND_ROOT_PATH
  )

  if(NOT OpenCV_INCLUDE_DIR OR NOT OpenCV_WORLD_LIB)
    message(FATAL_ERROR "Could not find custom OpenCV under OPENCV_ROOT=${OPENCV_ROOT}")
  endif()

  add_library(depends::opencv INTERFACE IMPORTED GLOBAL)
  target_include_directories(depends::opencv INTERFACE "${OpenCV_INCLUDE_DIR}")
  target_link_libraries(depends::opencv INTERFACE "${OpenCV_WORLD_LIB}")

  if(IOS)
    target_link_libraries(depends::opencv INTERFACE
      "-framework Accelerate"
      "-framework CoreFoundation"
      "-framework CoreGraphics"
      "-framework CoreImage"
      "-framework CoreMedia"
      "-framework CoreVideo"
      "-framework Foundation"
      "-framework UIKit"
      z
    )
  endif()
endif()
