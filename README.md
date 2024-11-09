# libmptask Example Usage Application

See [libmptask](https://github.com/XingRay/mediapipe) for more information on libmptask

[Mediapipe](https://github.com/google-ai-edge/mediapipe)



## How to

### 1 build env

#### 1 visual studio community

https://visualstudio.microsoft.com/downloads/



#### 2 cmake

install last version

https://cmake.org/

https://github.com/Kitware/CMake/releases/download/v3.31.0/cmake-3.31.0-windows-x86_64.msi



#### 3 opencv

version: 4.6.0

https://opencv.org/releases/

https://sourceforge.net/projects/opencvlibrary/



### 2 clone and build libmptask

see https://github.com/XingRay/mediapipe



### 3 edit CMakeLists.txt

```cmake
set(MEDIAPIPE_DIR "D:/code/git/github/mediapipe")
set(bazel_out_DIR "D:/code/git/github/mediapipe/bazel_out/uwisdqlp")
```

update to your paths



### 4 build and run main.cpp

prepare a face image, like:

```
D:/img/face_image.png
```

download mediapipe task file:

https://ai.google.dev/edge/mediapipe/solutions/vision/face_landmarker

https://storage.googleapis.com/mediapipe-models/face_landmarker/face_landmarker/float16/latest/face_landmarker.task

save in path, like:

```
D:/dev/face_landmarker.task
```

edit `main.cpp` like:

```cpp
const std::string model_path = "D:/dev/face_landmarker.task";
const std::string image_path = "D:/img/face_image.png";
```



generate

```shell
cmake -S . -B ./cmake-build-release -G "Visual Studio 17 2022"
```

build

```shell
cmake --build ./cmake-build-release --config Release
```

run

```
.\cmake-build-release\Release\main.exe 
```




## Thanks for

[Mediapipe](https://github.com/google-ai-edge/mediapipe)

[LibMP](https://github.com/rajkundu/mediapipe)

[libmp-example](https://github.com/rajkundu/libmp-example)
