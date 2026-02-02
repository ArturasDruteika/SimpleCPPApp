add_executable(SimpleCPPApp
    ${CMAKE_CURRENT_SOURCE_DIR}/../main.cpp
)

target_link_libraries(SimpleCPPApp
    PRIVATE core math text time
)

set_target_properties(SimpleCPPApp PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set_target_properties(SimpleCPPApp PROPERTIES
    BUILD_RPATH "$ORIGIN"
    INSTALL_RPATH "$ORIGIN"
)
