add_library(fundamentum STATIC)

target_sources(fundamentum
    PRIVATE
    "Logger.h"
    "Logger.cpp"
)

target_include_directories(fundamentum PUBLIC ${CMAKE_SOURCE_DIR})
target_link_libraries(fundamentum PRIVATE glm::glm)
