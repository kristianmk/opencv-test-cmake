# K. M. Knausgård 2024
cmake_minimum_required(VERSION 3.30)
project(opencv-test VERSION 0.1.0 LANGUAGES C CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)       # No GNU extensions

# Include FetchContent module
include(FetchContent)

# Set options to disable unnecessary modules before fetching OpenCV
set(BUILD_opencv_video ON)          # Enable the video module for tracking
set(BUILD_opencv_imgproc ON)        # Enable the image processing module (required by video module)
set(BUILD_opencv_imgcodecs ON)      # Enable the image processing module (required by video module)


set(BUILD_opencv_videoio OFF)
set(BUILD_opencv_objdetect OFF)
set(WITH_OBSENSOR OFF)
set(BUILD_EXAMPLES OFF)
set(BUILD_TESTS OFF)
set(BUILD_PERF_TESTS OFF)
set(BUILD_DOCS OFF)
set(BUILD_opencv_apps OFF)
set(WITH_FFMPEG OFF)
set(WITH_TBB OFF)
set(WITH_GAPI_TBB_EXECUTOR OFF)
set(BUILD_opencv_gapi OFF)
set(BUILD_ZLIB OFF)


# Fetch OpenCV version 4.10.0
FetchContent_Declare(
  opencv
  GIT_REPOSITORY https://github.com/opencv/opencv.git
  GIT_TAG        4.10.0
  GIT_PROGRESS TRUE
  GIT_SHALLOW TRUE
  OVERRIDE_FIND_PACKAGE
)

# Fetch and make OpenCV available
FetchContent_MakeAvailable(opencv)

# !! Should normally unset opencv related options here, as some could cause conflicts with other libraries if any.

# Define the executable
add_executable(${PROJECT_NAME} main.cpp)

# Link the required OpenCV libraries to the project
target_link_libraries(${PROJECT_NAME} PRIVATE opencv_core opencv_highgui opencv_video)

# Link the project with the installed OpenCV include and library paths
target_include_directories(${PROJECT_NAME} PRIVATE
  ${OPENCV_CONFIG_FILE_INCLUDE_DIR}
  ${OPENCV_MODULE_opencv_core_LOCATION}/include
  ${OPENCV_MODULE_opencv_highgui_LOCATION}/include
  ${OPENCV_MODULE_opencv_video_LOCATION}/include
  ${OPENCV_MODULE_opencv_imgproc_LOCATION}/include
  ${OPENCV_MODULE_opencv_imgcodecs_LOCATION}/include
)
