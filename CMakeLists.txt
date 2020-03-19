cmake_minimum_required(VERSION 3.10.2)
project(video_streaming_opencv_wt LANGUAGES CXX)

find_package(OpenCV REQUIRED)
find_package(Wt REQUIRED Wt HTTP)

add_executable(main ./main.cpp)
target_compile_features(main PRIVATE cxx_std_14)
target_link_libraries(main ${OpenCV_LIBS} Wt::Wt Wt::HTTP)
