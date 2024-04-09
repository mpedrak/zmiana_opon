cmake_minimum_required(VERSION 3.24)
project(SprawdzanieTemperatur)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

add_executable(main main.cpp)

find_package(RapidJSON CONFIG REQUIRED)
target_link_libraries(main PRIVATE rapidjson)

find_package(CURL CONFIG REQUIRED)
target_link_libraries(main PRIVATE CURL::libcurl)
