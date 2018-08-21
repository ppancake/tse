cmake_minimum_required(VERSION 3.12)
project(Search)

set(CMAKE_CXX_STANDARD 14)

add_executable(Search Search.cpp Query.h Document.h StrFun.h Dict.h HzSeg.h DisplayRst.h Dict.cpp HzSeg.cpp StrFun.cpp DisplayRst.cpp Document.cpp Query.cpp)