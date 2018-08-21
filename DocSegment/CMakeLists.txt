cmake_minimum_required(VERSION 3.12)
project(DocSegment)

set(CMAKE_CXX_STANDARD 14)

add_executable(DocSegment DocSegment.cpp MD5.h MD5.cpp Url.h Url.cpp Document.h Document.cpp HzSeg.h Dict.h Dict.cpp HzSeg.cpp StrFun.h StrFun.cpp)