cmake_minimum_required(VERSION 3.9)
project(mandelbrot)

ADD_DEFINITIONS(-DBOOST_LOG_DYN_LINK)

set(CMAKE_CXX_STANDARD 17)

set(EXECUTABLE_NAME "mandelbrot")
set(SRCS
        main.cpp StandardMandelbrotSet.cpp MandelbrotSet.cpp Buddhabrot.cpp)

add_compile_options(-Wall -Wextra -g)

find_package (Threads)
find_package (Boost 1.66 COMPONENTS log program_options REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
add_executable(${EXECUTABLE_NAME} ${SRCS})
target_link_libraries (${EXECUTABLE_NAME} ${CMAKE_THREAD_LIBS_INIT} ${Boost_LIBRARIES})

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})
find_package(SFML 2 REQUIRED system window graphics network audio)
if(SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(${EXECUTABLE_NAME} ${SFML_LIBRARIES})
endif()

