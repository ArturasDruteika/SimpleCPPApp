add_library(time SHARED
    src/clock.cpp
    src/time_format_helpers.cpp
)

target_include_directories(time PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include_public
)

target_include_directories(time PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include_private
)

set_target_properties(time PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
