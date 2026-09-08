
add_library(utility
    SWSRSlidingWindow.cpp
    LockFreeQueue.cpp
)

target_include_directories(connection PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
