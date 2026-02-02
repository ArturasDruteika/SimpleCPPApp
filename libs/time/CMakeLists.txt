add_library(time SHARED
    src/clock.cpp
)

target_include_directories(time PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(time PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
