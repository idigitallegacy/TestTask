cmake_minimum_required(VERSION 3.10.2)
project(TestTask)

set(CMAKE_CXX_STANDARD 17)

add_executable(Task_2.6 Task_2.6/main.cpp
        Task_2.6/class/Exception/IOException.cpp Task_2.6/class/Exception/IOException.h
        Task_2.6/class/WAVOutputGenerator.cpp Task_2.6/class/WAVOutputGenerator.h
        Task_2.6/class/TrackCompilerInterface.cpp Task_2.6/class/TrackCompilerInterface.h
        Task_2.6/class/TrackCompliler.cpp Task_2.6/class/TrackCompliler.h)

add_executable(Task_2.4 Task_2.4/main.cpp
        Task_2.6/class/WAVOutputGenerator.cpp Task_2.6/class/WAVOutputGenerator.h
        Task_2.6/class/TrackCompilerInterface.cpp Task_2.6/class/TrackCompilerInterface.h
        Task_2.6/class/TrackCompliler.cpp Task_2.6/class/TrackCompliler.h
        Task_2.4/class/StereoSoundGeneratorInterface.cpp Task_2.4/class/StereoSoundGeneratorInterface.h
        Task_2.4/class/StereoSoundGenerator.cpp Task_2.4/class/StereoSoundGenerator.h)

add_executable(Task_2.3 Task_2.3/main.cpp)