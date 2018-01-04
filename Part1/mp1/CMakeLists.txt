cmake_minimum_required(VERSION 3.9)
project(mp1)

set(CMAKE_CXX_STANDARD 11)

add_executable(mp1
        Application.cpp
        Application.h
        EmulNet.cpp
        EmulNet.h
        Log.cpp
        Log.h
        Member.cpp
        Member.h
        MP1Node.cpp
        MP1Node.h
        Params.cpp
        Params.h
        Queue.h
        stdincludes.h)
