add_library(text SHARED
    src/format.cpp
)

target_include_directories(text PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

set_target_properties(text PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
