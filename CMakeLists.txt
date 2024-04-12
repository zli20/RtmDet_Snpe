cmake_minimum_required(VERSION 3.10)
project(rtm_pose_snpe)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -Ofast -DNDEBUG -fopenmp")

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

FILE(GLOB HEAD_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
FILE(GLOB SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
set(INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")

#find_package( OpenCV REQUIRED )
#message(STATUS "OpenCV_DIR = ${OpenCV_DIR}")
#message(STATUS "OpenCV_INCLUDE_DIRS = ${OpenCV_INCLUDE_DIRS}")
#message(STATUS "OpenCV_LIBS = ${OpenCV_LIBS}")
#include_directories( ${OpenCV_INCLUDE_DIRS} )

include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/snpe2.10/include/zdl
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/Opencv4.6/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src)

link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/snpe2.10/lib/x86_64-linux-clang
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/Opencv4.6/lib
)
elseif(CMAKE_SYSTEM_NAME STREQUAL "Android")
include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/snpe1.68/include/zdl
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/opencv-4.6.0-android-sdk/OpenCV-android-sdk/sdk/native/jni/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src)
link_directories(
        # ${CMAKE_CURRENT_SOURCE_DIR}/third_party/snpe2.10/lib/aarch64-android-clang8.0
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/snpe2.10/lib/android
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/opencv-4.6.0-android-sdk/OpenCV-android-sdk/sdk/native/staticlibs/arm64-v8a
        ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/opencv-4.6.0-android-sdk/OpenCV-android-sdk/sdk/native/3rdparty/libs/arm64-v8a
)
endif()


add_executable(rtm_pose_snpe ${SRC_FILES})

target_link_libraries(rtm_pose_snpe libSNPE.so libUdoHardSigmodUdoPackageImplCpu.so)  #
#set(OpenCV_LIBS opencv_calib3d opencv_core opencv_dnn opencv_features2d opencv_flann opencv_gapi opencv_highgui opencv_imgcodecs opencv_imgproc opencv_ml
#        opencv_objdetect opencv_photo opencv_stitching opencv_videoio opencv_video)
set(OpenCV_LIBS opencv_core opencv_dnn opencv_highgui opencv_imgcodecs opencv_imgproc)
target_link_libraries(rtm_pose_snpe ${OpenCV_LIBS})
if(CMAKE_SYSTEM_NAME STREQUAL "Android")
#    set(OpenCV_Third_LIBS log tegra_hal cpufeatures tbb ittnotify IlmImf libjpeg-turbo libpng libopenjp2 libtiff libwebp quirc libprotobuf ade)
    set(OpenCV_Third_LIBS tegra_hal log tbb ittnotify IlmImf libopenjp2 libjpeg-turbo libpng libtiff libwebp z)
#    set(SNPE_Third_LIBS libc++_shared.so libPlatformValidatorShared.so libcalculator.so libcalculator_htp.so libhta_hexagon_runtime_snpe.so libSnpeDspV65Stub.so libSnpeDspV66Stub.so
#                        libSnpeHta.so libSnpeHtpPrepare.so libSnpeHtpV68Stub.so libSnpeHtpV69Stub.so libSnpeHtpV73Stub.so)
    set(SNPE_Third_LIBS libc++_shared.so) # libsnpe-android.so
    target_link_libraries(rtm_pose_snpe ${OpenCV_Third_LIBS} ${SNPE_Third_LIBS})
endif()






