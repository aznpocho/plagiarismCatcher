cmake_minimum_required(VERSION 3.12)
project(plagiarismCatcher)

set(CMAKE_CXX_STANDARD 14)

add_executable(plagiarismCatcher main.cpp TextList.cpp TextList.h)