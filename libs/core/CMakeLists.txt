add_library(core SHARED
    src/logger.cpp
    src/formatting.cpp
)

target_include_directories(core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include_public
)

target_include_directories(core PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include_private
)

set_target_properties(core PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
