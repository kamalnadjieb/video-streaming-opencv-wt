# Video Streaming with OpenCV and Wt

## Prerequisites
* C++14 or above
* [CMake 3.10.2 or above](https://cmake.org/)
* [OpenCV 4](https://opencv.org/)
* [Wt 4](https://www.webtoolkit.eu/wt)

## Build Project
```bash
mkdir build
cd build
cmake ..
make
```

## Run Project
```bash
./main --docroot . --http-address 0.0.0.0 --http-port 9090
```
