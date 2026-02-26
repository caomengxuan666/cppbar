cmake_minimum_required(VERSION 3.19)
project(cppbar VERSION 1.0.0 LANGUAGES CXX)

# ===== CMakeHub Integration =====
# Include CMakeHub loader
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/hub/loader.cmake)

# Use CMakeHub modules for code quality
cmakehub_use(cpp_standards)
cmakehub_use(compiler_warnings)
cmakehub_use(code_formatter)
cmakehub_use(clang_tidy_tools)

# Build optimization modules
cmakehub_use(lto_optimization)
cmakehub_use(compile_options)

# Sanitizers for Debug builds only
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    cmakehub_use(sanitizers)
    cmakehub_use(coverage)
endif()

# ===== Header-only library setup =====
# Create library (with some source files for platform implementations)
add_library(${PROJECT_NAME})

# Set include directory
target_include_directories(${PROJECT_NAME} PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Add source files for platform-specific implementations
target_sources(${PROJECT_NAME} PRIVATE
    src/terminal_unix.cpp
    src/terminal_win.cpp
)

# Link with required system libraries
if(WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC)
endif()

# Apply compiler warnings to targets that use this library
target_compile_options(${PROJECT_NAME} INTERFACE
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall -Wextra -Wpedantic>
)

# Platform-specific optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} INTERFACE
        $<$<CXX_COMPILER_ID:MSVC>:/O2>
        $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-O3>
    )
endif()

# Platform-specific definitions
if(WIN32)
    target_compile_definitions(${PROJECT_NAME} INTERFACE
        _CRT_SECURE_NO_WARNINGS
        NOMINMAX
    )
elseif(APPLE)
    target_compile_definitions(${PROJECT_NAME} INTERFACE
        _DARWIN_C_SOURCE
    )
endif()

# Enable testing
enable_testing()

# Add examples directory
add_subdirectory(examples)

# Add tests directory
add_subdirectory(tests)

# Installation rules for vcpkg
include(GNUInstallDirs)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}-targets
)

install(EXPORT ${PROJECT_NAME}-targets
    FILE ${PROJECT_NAME}-config.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}
)
