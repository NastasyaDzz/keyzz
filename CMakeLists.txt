cmake_minimum_required(VERSION 3.16)

project(DavePrototype VERSION 0.0.1 LANGUAGES CXX)

if(NOT CMAKE_CXX_EXTENSIONS)
    set(CMAKE_CXX_EXTENSIONS OFF)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3") # turn on compiller optimization

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Werror -Wextra -Wzero-as-null-pointer-constant -Winit-self -Wswitch-default -Wfloat-equal -Wno-unused-parameter -fPIC -march=native")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")  # debug symbols

add_definitions(-DELPP_DEFAULT_LOG_FILE="keyzz.log")
IF(CMAKE_BUILD_TYPE MATCHES Release)
    message("# Release mode")
    add_definitions(-DELPP_NO_DEFAULT_LOG_FILE)
ENDIF(CMAKE_BUILD_TYPE MATCHES Release)
#add_definitions(-DELPP_NO_LOG_TO_FILE)
#add_definitions(-DELPP_DISABLE_LOGS)  # Doesn't help - creates an empty log file.

add_subdirectory(logger)
add_subdirectory(minunity)
add_subdirectory(keyzz)
