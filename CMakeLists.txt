cmake_minimum_required(VERSION 3.10.2)
project(TestTask)

set(CMAKE_CXX_STANDARD 17)

add_executable(TestTask main.cpp class/WAVOutputGenerator.cpp class/WAVOutputGenerator.h class/Exception/IOException.cpp class/Exception/IOException.h class/TrackCompilerInterface.cpp class/TrackCompilerInterface.h class/TrackCompliler.cpp class/TrackCompliler.h)
