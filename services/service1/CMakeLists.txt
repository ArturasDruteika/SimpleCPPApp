add_executable(service1
    main.cpp
)

target_link_libraries(service1
    PRIVATE core math time
)

set_target_properties(service1 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)

set_target_properties(service1 PROPERTIES
    BUILD_RPATH "$ORIGIN"
    INSTALL_RPATH "$ORIGIN"
)
