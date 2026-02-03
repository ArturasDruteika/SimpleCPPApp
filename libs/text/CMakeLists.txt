add_library(text SHARED
    src/format.cpp
    src/delimiter_helpers.cpp
)

target_include_directories(text PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include_public
)

target_include_directories(text PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include_private
)

set_target_properties(text PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
