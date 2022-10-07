add_library(
    logger
    src/logger/easylogging++.cc
    )

set (CMAKE_CXX_FLAGS "-Wfloat-equal -DELPP_FRESH_LOG_FILE")
target_include_directories(logger PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src")
