add_library(math SHARED
    src/aggregates.cpp
    src/stats_helpers.cpp
)

target_include_directories(math PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include_public
)

target_include_directories(math PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include_private
)

set_target_properties(math PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
