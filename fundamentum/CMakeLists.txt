add_library(fundamentum STATIC)

target_sources(fundamentum
    PUBLIC
    "Logger.h"
    PRIVATE
    "Logger.cpp"
)

target_include_directories(fundamentum PUBLIC ${CMAKE_SOURCE_DIR})
