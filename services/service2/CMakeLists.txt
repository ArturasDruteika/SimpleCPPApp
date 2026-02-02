add_executable(service2
    main.cpp
)

target_link_libraries(service2
    PRIVATE core text time
)

set_target_properties(service2 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set_target_properties(service2 PROPERTIES
    BUILD_RPATH "$ORIGIN"
    INSTALL_RPATH "$ORIGIN"
)
