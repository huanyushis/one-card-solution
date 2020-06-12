cmake_minimum_required(VERSION 3.16)
project(untitled)

set(CMAKE_CXX_STANDARD 14)
include_directories(F:/mysql-8.0.19-winx64/include)
link_directories(F:/mysql-8.0.19-winx64/lib)
link_libraries(libmysql.dll)
zz
add_executable(untitled main.cpp)
target_link_libraries(untitled libmysql.dll)
